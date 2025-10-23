/**
 * @file mcp2515.c
 * @author author
 * @brief brief
 * @date 2025-10-09
 */

#include "mcp2515.h"
// clang-format off
#include "../constants.h"
// clang-format on

#include <util/delay.h>

#include "../gpio/gpio.h"
#include "../oled/oled.h"
#include "../spi/spi.h"
#include "mcp2515_const.h"

#define SPI_TRANSMIT(tranfer) (transmit_done = false, spi_transfer(&tranfer))

#define MCP_TXREQ 0x08

#define DDR_CAN_INTERRUPT  DDRB   // TODO find place
#define PORT_CAN_INTERRUPT PORTB  // TODO find place
#define PIN_CAN_INTERRUPT  PIN0   // TODO find place

#define DDR_CAN_RST  DDRD
#define PORT_CAN_RST PORTD
#define PIN_CAN_RST  PIN2

#define BUFFER_SIZE 8U
#define DUMMY       ((uint8_t)42)

static volatile bool transmit_done = true;
static uint8_t _transmit_buffer[BUFFER_SIZE];
static void _spi_transfer_cmplt(void* param);

static spi_transfer_t _transfer = {.rx_data = NULL,
                                   .tx_data = _transmit_buffer,
                                   .length = 0,
                                   .slave_idx = spi_slave_can,
                                   .transfer_cmplt_cbk = _spi_transfer_cmplt};

static void _spi_transfer_cmplt(void* param) {
    (void)param;  // unused
    transmit_done = true;
}

/** @brief Initialize the MCP2515 */
void mcp2515_init() {
    mcp2515_reset();  // Send reset-command
}

bool mcp2515_bit_modify(uint8_t port, uint8_t bit_mask, uint8_t data) {
    if (!transmit_done) return false;

    _transmit_buffer[0] = MCP_BITMOD;  // 0x05
    _transmit_buffer[1] = port;
    _transmit_buffer[2] = bit_mask;
    _transmit_buffer[3] = data;

    _transfer.tx_data = _transmit_buffer;
    _transfer.length = 4;

    return SPI_TRANSMIT(_transfer);
}

bool mcp2515_write(uint8_t tx_data, uint8_t address, uint8_t length) {
    if (!transmit_done || !tx_data) return false;

    _transmit_buffer[0] = MCP_WRITE;
    _transmit_buffer[1] = address;
    _transmit_buffer[2] = tx_data;  // DB maybe change to ptr at a later time
    _transfer.length = length + 2;
    _transfer.tx_data = _transmit_buffer;
    return SPI_TRANSMIT(_transfer);
}

/**
 * @brief does some reading i think
 * @param address i think this is the address we want to read from
 */
bool mcp2515_read(uint8_t* rx_data, uint8_t address) {
    if (!transmit_done) return false;

    _transmit_buffer[0] = MCP_READ;
    _transmit_buffer[1] = address;
    _transmit_buffer[2] = DUMMY;  // Just write dummy byte
    _transfer.length = 3;
    _transfer.tx_data = _transmit_buffer;
    _transfer.rx_data = rx_data;
    return SPI_TRANSMIT(_transfer);
}

bool mcp2515_read_status(uint8_t* rx_data) {
    if (!transmit_done) return false;

    _transmit_buffer[0] = MCP_READ_STATUS;
    _transmit_buffer[1] = DUMMY;
    _transfer.length = 2;
    _transfer.tx_data = _transmit_buffer;
    _transfer.rx_data = rx_data;
    return SPI_TRANSMIT(_transfer);
}

/** @brief Resets the OLED display */
void mcp2515_reset(void) {
    CLEAR_PIN(PORT_CAN_RST, PIN_CAN_RST);
    _delay_us(10);
    SET_PIN(PORT_CAN_RST, PIN_CAN_RST);
    _delay_us(10);
}

/**
 * @brief Request to send
 * @param address Which RST_TXc we want to use
 */
bool mcp2515_request_to_send(uint8_t address) {
    if (!transmit_done) return false;

    _transfer.length = 1;
    _transmit_buffer[0] = address;
    _transfer.tx_data = _transmit_buffer;
    return SPI_TRANSMIT(_transfer);
}