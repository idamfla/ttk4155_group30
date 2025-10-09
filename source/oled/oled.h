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

/** @brief Initialize the oled */
void oled_init(void);

/**
 * @brief Sets start page and start column
 * @param page index of the page we want to write on
 * @param column the column we want to start to write on
 */
void oled_go_to_page_and_column(uint8_t page, uint8_t col);

/** @brief Resets the OLED display */
void oled_reset_display(void);

/**
 * @brief Writes to the display
 * @param tx_data pointer to the data we want to send
 * @param length the length data length
 * @param transfer_cmplt_cbk the callback function
 * @param param the parameter for the callback function
*/
void oled_write_to_display(uint8_t* tx_data, uint8_t length, void (*transfer_cmplt_cbk)(void* param),
                        void* param);

/** @brief Sets every pixel off */
void oled_clean_display(void);