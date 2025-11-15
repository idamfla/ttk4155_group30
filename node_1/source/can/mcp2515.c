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

#include <avr/interrupt.h>
#include <util/delay.h>

#include "../gpio/gpio.h"
#include "../oled/oled.h"
#include "../spi/spi.h"
#include "mcp2515_const.h"

#define SPI_TRANSMIT(tranfer) (transmit_done = false, spi_transfer(&tranfer))

#define MCP_TXREQ 0x08

#define DDR_CAN_INTERRUPT  DDRB   // TODO find place
#define PORT_CAN_INTERRUPT PORTB  // TODO find place
#define PIN_CAN_INTERRUPT  PIN3   // TODO find place

#define BUFFER_SIZE 8U

static volatile bool transmit_done = true;
static uint8_t _transmit_buffer[BUFFER_SIZE];
static void _spi_transfer_cmplt(void* param);
void mcp2515_interrupt_enable();

static spi_transfer_t _transfer = {
    .rx_data = NULL,
    .tx_data = _transmit_buffer,
    .length = 0,
    .slave_idx = spi_slave_can,
    .transfer_cmplt_cbk = _spi_transfer_cmplt,
    .transfer_start_cbk = NULL,
};

static void _spi_transfer_cmplt(void* param) {
    (void)param;  // unused
    transmit_done = true;
}

/** @brief Initialize the MCP2515 */
void mcp2515_init() {
    mcp2515_reset();  // Send reset-command
}

/** @brief Changes specific bits in the can controller
 * @param port address to register
 * @param bit_mask select which bits to change
 * @param data the value it gets changed to
 */
bool mcp2515_bit_modify(uint8_t port, uint8_t bit_mask, uint8_t data) {
    if (!transmit_done) return false;

    _transmit_buffer[0] = MCP_BITMOD;  // 0x05
    _transmit_buffer[1] = port;
    _transmit_buffer[2] = bit_mask;
    _transmit_buffer[3] = data;

    _transfer.tx_data = _transmit_buffer;
    _transfer.rx_data = NULL;
    _transfer.length = 4;

    return SPI_TRANSMIT(_transfer);
}

/** @brief Returns the state of the transmision */
bool mcp2515_transmit_done() {
    if (MCP_CANINTF != 0) {
        mcp2515_bit_modify(MCP_CANINTF, 0xFF, 0x00);
        while (!transmit_done);
    }
    return transmit_done;
}

/** @brief Writes data to a selected register in the can controller
 * @param tx_data an array of the data that is being sent
 * @param address Address to the register
 * @param length Length of the data being sent
 */
bool mcp2515_write(volatile uint8_t* tx_data, uint8_t address, uint8_t length) {
    if (!transmit_done || !tx_data) return false;

    _transmit_buffer[0] = MCP_WRITE;
    _transmit_buffer[1] = address;
    for (uint8_t i = 0; i < length; i++) {
        _transmit_buffer[2 + i] = tx_data[i];
    }
    _transfer.length = length + 2;
    _transfer.tx_data = _transmit_buffer;
    _transfer.rx_data = NULL;
    return SPI_TRANSMIT(_transfer);
}

/**
 * @brief Reads from a given register in the can controller
 * @param rx_data Pointer to where the data should be stored
 * @param address Address being read from
 * @param length Length of the data being read
 */
bool mcp2515_read(volatile uint8_t* rx_data, uint8_t address, uint8_t length) {
    if (!transmit_done) return false;

    _transmit_buffer[0] = MCP_READ;
    _transmit_buffer[1] = address;
    for (uint8_t i = 0; i < length; i++) {
        _transmit_buffer[2 + i] = DUMMY;  // Just write dummy byte
    }
    _transfer.length = 2 + length;
    _transfer.tx_data = _transmit_buffer;
    _transfer.rx_data = rx_data;
    return SPI_TRANSMIT(_transfer);
}

/** @brief Returns the state of the reading
 * @param rx_data Pointer to where the status should be stored
 */
bool mcp2515_read_status(uint8_t* rx_data) {
    if (!transmit_done) return false;

    _transmit_buffer[0] = MCP_READ_STATUS;
    _transmit_buffer[1] = DUMMY;
    _transfer.length = 2;
    _transfer.tx_data = _transmit_buffer;
    _transfer.rx_data = rx_data;
    return SPI_TRANSMIT(_transfer);
}

/** @brief Sends reset instruction*/
void mcp2515_reset(void) {
    _transmit_buffer[0] = MCP_RESET;
    _transfer.length = 1;
    _transfer.tx_data = _transmit_buffer;
    _transfer.rx_data = NULL;
    SPI_TRANSMIT(_transfer);
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
    _transfer.rx_data = NULL;
    return SPI_TRANSMIT(_transfer);
}