/**
 * @file spi.h
 * @author Thomas Berg
 * @author Ida Flakstad
 * @brief brief
 * @date 2025-09-29
 */

#pragma once
#include <avr/io.h>
#include "spi_queue.h"
#include "spi.h"

//idk
#define PORT_OLED_RST PORTB
#define PIN_OLED_RST  //somewhere

#define DDR_OLED_DC DDRB
#define PORT_OLED_DC PORTB
#define PIN_OLED_DC  // some place it is room

#define DDR_OLED_SS DDRB
#define PORT_OLED_SS PORTB
#define PIN_OLED_SS  SS2

void oled_send_command(uint8_t cmd);
void oled_reset(void);
void oled_init(void);

//initialization commands for the OLED display for the SSD1308 driver not the SSD1309
//Maybe change
const uint8_t init_cmds[] = {
    0xAE,       // Display OFF
    0xA1,       // Segment remap
    0xDA, 0x12, // COM pins config
    0xC8,       // COM scan direction remap
    0xA8, 0x3F, // Multiplex ratio (64)
    0xD5, 0x80, // Set display clock divide ratio/oscillator
    0x81, 0x50, // Contrast
    0xD9, 0x21, // Pre-charge period
    0x20, 0x02, // Memory mode = horizontal
    0xDB, 0x30, // VCOMH deselect level
    0XAD, 0x00, // Master config
    0xA4,       // Resume to RAM display
    0xA6,       // Normal display (not inverted)
    0xAF        // Display ON
};