/**
 * @file mcp2515.c
 * @author Marius Bär
 * @brief Driver for the MCP2515 CAN controller
 * @date 2025-11-16
 */

#include "mcp2515.h"

#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../spi/spi.h"

#define BUFFER_SIZE 10U

static void transfer_cmplt(void* unused);

typedef struct {
    union {
        void (*get_cmplt_cbk)(volatile void* data);
        void (*set_cmplt_cbk)(void);
    };
    bool get;
} transfer_param_t;

static volatile uint8_t _rx_data[BUFFER_SIZE];
static uint8_t _tx_data[BUFFER_SIZE];
static transfer_param_t _transfer_cmplt_param;
static volatile bool _transfer_active = false;

static spi_transfer_t _transfer = {
    .slave_idx = spi_slave_can,
    .transfer_cmplt_cbk = transfer_cmplt,
    .cmplt_param = NULL,
    .transfer_start_cbk = NULL,
};

static inline bool lock_transfer(void) {
    // bool lock_successful;
    // cli();
    // if (_transfer_active) {
    //     lock_successful = false;
    // } else {
    //     _transfer_active = true;
    //     lock_successful = true;
    // }
    //  sei();
    // return lock_successful;

    // It should work without a guard because MCP2515 operations are only called from the CAN
    // state-machine, making concurrent access impossible.
    return true;
}

static void transfer_cmplt(void* unused) {
    (void)unused;  // unused
    if (_transfer_cmplt_param.get) {
        _transfer_cmplt_param.get_cmplt_cbk(_transfer.rx_data);
    } else {
        if (_transfer_cmplt_param.set_cmplt_cbk) {
            _transfer_cmplt_param.set_cmplt_cbk();
        }
    }
    _transfer_active = false;
}

/**·
 * @attention The first two bytes of tx_data will be overwritten!
 * @param length Length of the data being sent (excluding the 2 bytes for command and address)
 */
bool mcp2515_write(uint8_t* tx_data, uint8_t address, uint8_t length,
                   mcp2515_write_cmplt_t write_cmplt_cbk) {
    if (!lock_transfer()) {
        return false;
    }

    tx_data[0] = MCP_WRITE;
    tx_data[1] = address;

    _transfer.tx_data = tx_data;
    _transfer.rx_data = NULL;
    _transfer.length = length + 2U;

    _transfer_cmplt_param.get = false;
    _transfer_cmplt_param.set_cmplt_cbk = write_cmplt_cbk;

    return spi_transfer(&_transfer);
}

bool mcp2515_read(uint8_t address, uint8_t length, mcp2515_read_cmplt_t read_cmplt_cbk) {
    if (!lock_transfer()) {
        return false;
    }

    _tx_data[0] = MCP_READ;
    _tx_data[1] = address;

    _transfer.tx_data = _tx_data;
    _transfer.rx_data = _rx_data;
    _transfer.length = length + 2U;

    _transfer_cmplt_param.get = true;
    _transfer_cmplt_param.get_cmplt_cbk = (void (*)(volatile void*))read_cmplt_cbk;

    return spi_transfer(&_transfer);
}

bool mcp2515_bit_modify(uint8_t port, uint8_t bit_mask, uint8_t data,
                        mcp2515_bit_modify_cmplt_t bit_modify_cmplt_cbk) {
    if (!lock_transfer()) {
        return false;
    }

    _tx_data[0] = MCP_BITMOD;  // 0x05
    _tx_data[1] = port;
    _tx_data[2] = bit_mask;
    _tx_data[3] = data;

    _transfer.tx_data = _tx_data;
    _transfer.rx_data = NULL;
    _transfer.length = 4;

    _transfer_cmplt_param.get = false;
    _transfer_cmplt_param.set_cmplt_cbk = bit_modify_cmplt_cbk;

    return spi_transfer(&_transfer);
}

bool mcp2515_read_status(mcp2515_read_status_cmplt_t bit_modify_cmplt_cbk) {
    if (!lock_transfer()) {
        return false;
    }

    _tx_data[0] = MCP_READ_STATUS;
    _tx_data[1] = 0x00;  // Dummy byte

    _transfer.tx_data = _tx_data;
    _transfer.rx_data = _rx_data;
    _transfer.length = 2;

    _transfer_cmplt_param.get = true;
    _transfer_cmplt_param.get_cmplt_cbk = (void (*)(volatile void*))bit_modify_cmplt_cbk;

    return spi_transfer(&_transfer);
}

bool mcp2515_reset(mcp2515_reset_cmplt_t reset_cmplt_cbk) {
    if (!lock_transfer()) {
        return false;
    }

    _tx_data[0] = MCP_RESET;

    _transfer.tx_data = _tx_data;
    _transfer.rx_data = NULL;
    _transfer.length = 1;

    _transfer_cmplt_param.get = false;
    _transfer_cmplt_param.set_cmplt_cbk = reset_cmplt_cbk;

    return spi_transfer(&_transfer);
}

bool mcp2515_request_to_send(uint8_t address, mcp2515_rts_cmplt_t rts_cmplt_cbk) {
    if (!lock_transfer()) {
        return false;
    }

    _tx_data[0] = address;

    _transfer.tx_data = _tx_data;
    _transfer.rx_data = NULL;
    _transfer.length = 1;

    _transfer_cmplt_param.get = false;
    _transfer_cmplt_param.set_cmplt_cbk = rts_cmplt_cbk;

    return spi_transfer(&_transfer);
}
