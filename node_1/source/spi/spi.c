/**
 * @file spi.c
 * @author Thomas Berg
 * @author Ida Flakstad
 * @brief brief
 * @date 2025-09-23
 */

#include "spi.h"

#include <avr/interrupt.h>
#include <string.h>

#include "../gpio/gpio.h"
#include "spi_queue.h"
#include "../can/mcp2515.h"

// #define DDR_SPI DDRB
// #define DD_MOSI DDB5
// #define DD_SCK  DDB7

// // #define SS DDB4
// #define DDR_SLAVES  DDRB
// #define PORT_SLAVES PORTB
// #define SS1         DDB3
// #define SS2         DDB4
// #define DUMMY_DATA  0x00

#define DDR_SPI_MOSI  DDRB
#define PORT_SPI_MOSI PORTB
#define PIN_SPI_MOSI  PB5

#define DDR_SPI_SCK  DDRB
#define PORT_SPI_SCK PORTB
#define PIN_SPI_SCK  PB7

#define DDR_OLED_DC  DDRB
#define PORT_OLED_DC PORTB
#define PIN_OLED_DC  PB2

#define DDR_OLED_SS  DDRB
#define PORT_OLED_SS PORTB
#define PIN_OLED_SS  PB3

#define DDR_IO_SS  DDRB
#define PORT_IO_SS PORTB
#define PIN_IO_SS  PB4

#define DDR_CAN_SS  DDRD
#define PORT_CAN_SS PORTD
#define PIN_CAN_SS  PD4

#define QUEUE_SIZE 10

static spi_transfer_t _spi_queue_buffer[QUEUE_SIZE];
static spi_queue_t _spi_queue = {.max_size = QUEUE_SIZE,
                                 .size = 0,
                                 .buffer = _spi_queue_buffer,
                                 .front = _spi_queue_buffer,
                                 .back = _spi_queue_buffer};
static volatile uint16_t _data_idx;
static spi_transfer_t _transfer;
static volatile bool _transfer_active = false;

static void _spi_slave_select(uint8_t slave_idx);
static void _spi_slave_deselect(uint8_t slave_idx);

static inline void _spi_rxtx(void) {
    if (_transfer.tx_data) {
        SPDR = _transfer.tx_data[_data_idx];
    } else {
        SPDR = 0;
    }
}

static void _spi_slave_select(uint8_t slave_idx) {
    switch (slave_idx) {
        case spi_slave_disp_d:
            CLEAR_PIN(PORT_OLED_SS, PIN_OLED_SS);
            SET_PIN(PORT_OLED_DC, PIN_OLED_DC);
            break;
        case spi_slave_disp_c:
            CLEAR_PIN(PORT_OLED_SS, PIN_OLED_SS);
            CLEAR_PIN(PORT_OLED_DC, PIN_OLED_DC);
            break;
        case spi_slave_io:
            CLEAR_PIN(PORT_IO_SS, PIN_IO_SS);
            break;
        case spi_slave_can:
            CLEAR_PIN(PORT_CAN_SS, PIN_CAN_SS);
            break;
        default:
            break;
    }
}

static void _spi_slave_deselect(uint8_t slave_idx) {
    switch (slave_idx) {
        case spi_slave_disp_d:
            SET_PIN(PORT_OLED_SS, PIN_OLED_SS);
            break;
        case spi_slave_disp_c:
            SET_PIN(PORT_OLED_SS, PIN_OLED_SS);
            break;
        case spi_slave_io:
            SET_PIN(PORT_IO_SS, PIN_IO_SS);
            break;
        case spi_slave_can:
            SET_PIN(PORT_CAN_SS, PIN_CAN_SS);
            break;
        default:
            break;
    }
}

void _spi_next_transfer(void) {
    if (_transfer_active || _spi_queue.size == 0) {
        return;
    }
    _transfer_active = true;
    _data_idx = 0;
    _transfer = spi_queue_pop(&_spi_queue);
    _spi_slave_select(_transfer.slave_idx);
    if (_transfer.transfer_start_cbk) {
        _transfer.transfer_start_cbk();
    }
    _spi_rxtx();
}

void spi_master_init(void) {
    /* Set MOSI and SCK output, all others input */
    DDR_IO_SS |= (1 << PIN_IO_SS);
    DDR_SPI_MOSI |= (1 << PIN_SPI_MOSI);
    DDR_SPI_SCK |= (1 << PIN_SPI_SCK);
    DDR_OLED_DC |= (1 << PIN_OLED_DC);
    DDR_OLED_SS |= (1 << PIN_OLED_SS);
    DDR_CAN_SS |= (1 << PIN_CAN_SS);

    // DDR_SPI &= ~(1 << PB6);  // MISO as input
    // Max frequency of slaves:
    // - MCP2515 (CAN): 10 MHz */
    // - SSD1309 (OLED): 10 MHz
    // - IO board: not specified (at least 10 MHz)
    /* Enable SPI, Master, set clock rate fck/4 (~1.2 MHz), interrupt*/
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPIE);  // | (1 << SPR0);

    /* Set slave pins as output, and set them high (SS\bar) */
    SET_PIN(PORT_IO_SS, PIN_IO_SS);
    SET_PIN(PORT_OLED_SS, PIN_OLED_SS);
    SET_PIN(PORT_CAN_SS, PIN_CAN_SS);
}

bool spi_transfer(const spi_transfer_t* transfer) {
    cli();
    if (spi_queue_push(&_spi_queue, transfer)) {
        _spi_next_transfer();
        sei();
        return true;
    }
    sei();
    return false;
}

void spi_ll_transmit_blocking(uint8_t data) {
    SPCR &= ~(1 << SPIE);  // Disable interrupt
    SPDR = data;
    while (!(SPSR & (1 << SPIF)));
    // Clear SPIF by reading SPSR (already read in while loop) and (!) SPDR
    // See manual page 164 (Bit 7 – SPIF: SPI Interrupt Flag)
    (void)SPDR;
    SPCR |= (1 << SPIE);  // Enable interrupt
}

ISR(SPI_STC_vect) {
    cli();
    if (_transfer.rx_data) {
        _transfer.rx_data[_data_idx] = SPDR;
    }
    ++_data_idx;
    if (_data_idx < _transfer.length) {
        _spi_rxtx();
    } else {
        _spi_slave_deselect(_transfer.slave_idx);
        _transfer_active = false;
        if (_transfer.transfer_cmplt_cbk) {
            _transfer.transfer_cmplt_cbk(_transfer.cmplt_param);
        }
        _spi_next_transfer();
    }
    sei();
}
