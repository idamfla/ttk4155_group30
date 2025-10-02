/**
 * @file spi.h
 * @author Thomas Berg
 * @author Ida Flakstad
 * @brief brief
 * @date 2025-09-29
 */

#pragma once
#include <avr/io.h>

#include "../spi/spi.h"
#include "../spi/spi_queue.h"

// idk
#define DDR_OLED_DC  DDRC
#define PORT_OLED_DC PORTC
#define PIN_OLED_DC  PC0

#define DDR_OLED_RST  PORTC
#define PORT_OLED_RST PORTC
#define PIN_OLED_RST  PC1

#define DDR_OLED_SS  DDRB
#define PORT_OLED_SS PORTB
#define PIN_OLED_SS  SS2

void _spi_transfer_cplt(void);
void _spi_transfer_started(void);
void oled_reset(void);
void oled_init(void);
void oled_go_to_page_and_column(uint8_t page, uint8_t col);

static const uint8_t init_cmds[] = {
    0xAE,  // Display OFF
    // 0xA1,        // Segment remap
    // 0xDA, 0x12,  // COM pins config
    0xC8,  // COM scan direction remap
    0x40,  // set ram to 0 or something
    // 0xA8, 0x3F,  // Multiplex ratio (64)
    // 0xD5, 0x80,  // Set display clock divide ratio/oscillator
    // 0x81, 0x50,  // Contrast
    // 0xD9, 0x21,  // Pre-charge period
    // 0x20, 0x00,  // Memory mode = horizontal
    // 0xDB, 0x30,  // VCOMH deselect level
    // 0XAD, 0x8B,  // Master config
    0xAF,  // Display ON
    0xA4,  // Resume to RAM display
    0xA6,  // Normal display (not inverted)
};
