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

void oled_init(void);

void oled_go_to_page_and_column(uint8_t page, uint8_t col);

void oled_reset(void);

void oled_write_to_display(uint8_t* tx_data, uint8_t length, void (*transfer_cmplt_cbk)(void* param),
                        void* param);

void oled_clear(void);