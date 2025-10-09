/**
 * @file spi.h
 * @author Thomas Berg
 * @author Ida Flakstad
 * @brief brief
 * @date 2025-09-29
 */

#include "oled.h"

#include <stdint.h>

#include "../gpio/gpio.h"

#define F_CPU 8000000UL  // or 16000000UL, whatever your MCU clock is
#include <util/delay.h>

#define DDR_OLED_RST  DDRD
#define PORT_OLED_RST PORTD
#define PIN_OLED_RST  PIN2

static volatile bool transmit_done = false;
static uint8_t _transmit_buffer[10];
static void _spi_transfer_cmplt(void* param);

static spi_transfer_t _transfer = {.rx_data = NULL,
                                   .tx_data = _transmit_buffer,
                                   .length = 0,
                                   .slave_idx = spi_slave_disp_c,
                                   .transfer_cmplt_cbk = _spi_transfer_cmplt};

static void _spi_transfer_cmplt(void* param) {
    (void)param;  // unused
    transmit_done = true;
}

void oled_init(void) {
    // Set pins as output

    // Put reset as output and turn reset high
    DDR_OLED_RST |= (1 << PIN_OLED_RST);
    SET_PIN(PORT_OLED_RST, PIN_OLED_RST);

    // send init commands
    uint8_t init_cmds_size = sizeof(init_cmds) / sizeof(init_cmds[0]);
    for (uint8_t i = 0; i < init_cmds_size; i++) {
        _transmit_buffer[i] = init_cmds[i];
    }
    _transfer.length = init_cmds_size;
    _transfer.slave_idx = spi_slave_disp_c;

    transmit_done = false;
    spi_transfer(&_transfer);

    while (!transmit_done) {
    }
    oled_clear_display();
}

void oled_go_to_page_and_column(uint8_t page, uint8_t col) {
    if ((page > 7) || (col > 127) || (!transmit_done)) {
        return;
    }

    _transmit_buffer[0] = 0x22;  // 0x22, set page address
    _transmit_buffer[1] = page;  // page, page start
    _transmit_buffer[2] = 0x07;  // 0x07, page end
    _transmit_buffer[3] = 0x21;  // 0x21, set column address
    _transmit_buffer[4] = col;   // col, column start
    _transmit_buffer[5] = 0x7F;  // 0x7f, coumn end
    _transfer.tx_data = _transmit_buffer;
    _transfer.length = 6;

    _transfer.slave_idx = spi_slave_disp_c;
    spi_transfer(&_transfer);

    // _transmit_buffer[0] = 0x40;    // data mode
    // _transmit_buffer[1] = 5;     // one byte: one pixel on
    // _transfer.length = 2;
    // _transfer.slave_idx = spi_slave_disp_d;
    // spi_transfer(&_transfer);
}

void oled_clear_display(void) {
    CLEAR_PIN(PORT_OLED_RST, PIN_OLED_RST);
    _delay_ms(10);
    SET_PIN(PORT_OLED_RST, PIN_OLED_RST);
}

void oled_write_to_disp(uint8_t* tx_data, uint8_t length, void (*transfer_cmplt_cbk)(void* param),
                        void* param) {
    if (!transmit_done) {
        return;
    }

    for (uint8_t i = 0; i < length; i++) {
        _transmit_buffer[i] = tx_data[i];
    }

    _transfer.tx_data = tx_data;
    _transfer.slave_idx = spi_slave_disp_d;
    _transfer.transfer_cmplt_cbk = transfer_cmplt_cbk;
    _transfer.param = param;
    _transfer.length = length;
    spi_transfer(&_transfer);
    return;
}
