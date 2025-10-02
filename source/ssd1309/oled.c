/**
 * @file spi.h
 * @author Thomas Berg
 * @author Ida Flakstad
 * @brief brief
 * @date 2025-09-29
 */

#include "oled.h"

#include <stdint.h>

#define F_CPU 8000000UL  // or 16000000UL, whatever your MCU clock is
#include <util/delay.h>

static volatile bool transmit_done = false;
static uint8_t _transmit_buffer[128];

static spi_transfer_t _transfer = {.rx_data = NULL,
                                   .tx_data = _transmit_buffer,
                                   .length = 0,
                                   .slave_idx = spi_slave_disp_c,
                                   .transfer_cmplt_cbk = _spi_transfer_cplt,
                                   .transfer_started = _spi_transfer_started};

void _spi_transfer_cplt(void) {
    transmit_done = true;
}

void _spi_transfer_started(void) {
    transmit_done = false;
}

void oled_reset(void) {
    PORT_OLED_RST &= ~(1 << PIN_OLED_RST);
    // _delay_ms(10);
    PORT_OLED_RST |= (1 << PIN_OLED_RST);
    // _delay_ms(10);
}

void oled_init(void) {
    // Set pins as output

    // oled_reset();

    // send init commands
    uint8_t init_cmds_size = sizeof(init_cmds) / sizeof(init_cmds[0]);
    for (uint8_t i = 0; i < init_cmds_size; i++) {
        _transmit_buffer[i] = init_cmds[i];
    }
    _transfer.length = init_cmds_size;
    _transfer.slave_idx = spi_slave_disp_c;
    spi_transfer(&_transfer);

    while (!transmit_done) {
    }
}

void oled_go_to_page_and_column(uint8_t page, uint8_t col) {
    if (page > 7) {
        return;
    }
    if (col > 127) {
        return;
    }

    if (!transmit_done) {
        return;
    }

    _transmit_buffer[0] = 0x22;
    _transmit_buffer[1] = page;
    _transmit_buffer[2] = page;
    _transmit_buffer[3] = 0x21;
    _transmit_buffer[4] = col;
    _transmit_buffer[5] = 0x7F;
    _transfer.length = 6;

    _transfer.slave_idx = spi_slave_disp_c;
    spi_transfer(&_transfer);
}