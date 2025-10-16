/**
 * @file spi.h
 * @author Thomas Berg
 * @author Ida Flakstad
 * @brief brief
 * @date 2025-09-29
 */

#include "oled.h"
// clang-format off
#include "../constants.h"
// clang-format on

#include <stdint.h>
#include <string.h>
#include <util/delay.h>

#include "../gpio/gpio.h"

#define SPI_TRANSMIT(tranfer)   \
    do {                        \
        transmit_done = false;  \
        spi_transfer(&tranfer); \
    } while (0);

#define DDR_OLED_RST  DDRD
#define PORT_OLED_RST PORTD
#define PIN_OLED_RST  PIN2
#define BUFFER_SIZE   8U

static const uint8_t init_cmds[] = {
    0xAE,  // Display OFF
    0xC8,  // COM scan direction remap
    0xA1,  // Segment remap
    0x20,  // Memory mode
    0x00,  // Memory mode = horizontal
    0xA4,  // Resume to RAM display
    0xA6,  // Normal display (not inverted)
};

static volatile bool transmit_done = false;
static uint8_t _transmit_buffer[BUFFER_SIZE];
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
    // Put reset as output and turn reset high
    DDR_OLED_RST |= (1 << PIN_OLED_RST);
    SET_PIN(PORT_OLED_RST, PIN_OLED_RST);

    oled_reset_display();

    // Set up init commands
    uint8_t init_cmds_size = sizeof(init_cmds) / sizeof(init_cmds[0]);
    for (uint8_t i = 0; i < init_cmds_size; i++) {
        _transmit_buffer[i] = init_cmds[i];
    }
    _transfer.length = init_cmds_size;
    _transfer.slave_idx = spi_slave_disp_c;

    // Send init commands
    SPI_TRANSMIT(_transfer);

    while (!transmit_done);

    oled_clean_display();

    // Ready display
    _transfer.length = 1;
    _transfer.slave_idx = spi_slave_disp_c;
    _transmit_buffer[0] = 0xAF;  // Display ON
    SPI_TRANSMIT(_transfer);
}

void oled_go_to_page_and_column(uint8_t page, uint8_t col) {
    if ((page > 7) || (col > 127) || (!transmit_done)) return;

    _transmit_buffer[0] = 0x22;  // 0x22, set page address
    _transmit_buffer[1] = page;  // page, page start
    _transmit_buffer[2] = 0x07;  // 0x07, page end
    _transmit_buffer[3] = 0x21;  // 0x21, set column address
    _transmit_buffer[4] = col;   // col, column start
    _transmit_buffer[5] = 0x7F;  // 0x7f, coumn end
    _transfer.tx_data = _transmit_buffer;
    _transfer.length = 6;

    _transfer.slave_idx = spi_slave_disp_c;
    SPI_TRANSMIT(_transfer);
}

void oled_reset_display(void) {
    CLEAR_PIN(PORT_OLED_RST, PIN_OLED_RST);
    _delay_us(10);
    SET_PIN(PORT_OLED_RST, PIN_OLED_RST);
    _delay_us(10);
}

void oled_write_to_display(uint8_t* tx_data, uint8_t length,
                           void (*transfer_cmplt_cbk)(void* param), void* param) {
    if (!transmit_done) return;

    _transfer.tx_data = tx_data;
    _transfer.slave_idx = spi_slave_disp_d;
    _transfer.transfer_cmplt_cbk = transfer_cmplt_cbk;
    _transfer.cmplt_param = param;
    _transfer.length = length;
    spi_transfer(&_transfer);
    return;
}

void oled_clean_display(void) {
    oled_go_to_page_and_column(0x00, 0x00);
    while (!transmit_done);
    memset(_transmit_buffer, 0U, BUFFER_SIZE);
    _transfer.slave_idx = spi_slave_disp_d;
    _transfer.length = BUFFER_SIZE;
    for (uint8_t i = 0; i < 8U * 128U / BUFFER_SIZE; i++) {
        SPI_TRANSMIT(_transfer);
        while (!transmit_done);
    }
}