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

#define PORT_OLED_DC PORTB
#define PIN_OLED_DC  PB2

#define PORT_OLED_SS PORTB
#define PIN_OLED_SS  PB3

#define PORT_IO_SS PORTB
#define PIN_IO_SS  PB4

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

static void _spi_rxtx(void) {
    SPDR = _transfer.tx_data[_data_idx++];
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
        default:
            break;
    }
}

static void _spi_slave_deselect(uint8_t slave_idx) {
    switch (slave_idx) {
        case spi_slave_disp_d:
            SET_PIN(PORT_OLED_SS, PIN_IO_SS);
            break;
        case spi_slave_disp_c:
            SET_PIN(PORT_OLED_SS, PIN_OLED_SS);
            break;
        case spi_slave_io:
            SET_PIN(PORT_IO_SS, PIN_IO_SS);
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
    _spi_rxtx();
}

void spi_master_init(void) {
    /* Set MOSI and SCK output, all others input */
    DDR_SPI = (1 << DD_MOSI) | (1 << DD_SCK);
    // DDR_SPI &= ~(1 << PB6);  // MISO as input
    /* Enable SPI, Master, set clock rate fck/16, interrupt*/
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0) | (1 << SPIE);

    /* Set slave pins as output, and set them high (SS\bar) */
    DDR_SLAVES |= (1 << SS1) | (1 << SS2);
    PORT_SLAVES |= (1 << SS1) | (1 << SS2);
}

void spi_transfer(const spi_transfer_t *transfer) {
    spi_queue_push(&_spi_queue, transfer);
    _spi_next_transfer();
}

ISR(SPI_STC_vect) {
    if (_transfer.rx_data) {
        _transfer.rx_data[_data_idx] = SPDR;
    }
    if (_data_idx < _transfer.length) {
        _spi_rxtx();
    } else {
        _spi_slave_deselect(_transfer.slave_idx);
        _transfer_active = false;
        _spi_next_transfer();
    }
}