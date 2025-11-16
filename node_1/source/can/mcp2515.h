/**
 * @file mcp2515.h
 * @author Marius Bär
 * @brief Header file for mcp2515.c
 * @date 2025-11-16
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "mcp2515_const.h"

typedef void (*mcp2515_write_cmplt_t)(void);
typedef void (*mcp2515_read_cmplt_t)(volatile uint8_t* data);
typedef void (*mcp2515_bit_modify_cmplt_t)(void);
typedef void (*mcp2515_read_status_cmplt_t)(uint8_t* status);
typedef void (*mcp2515_reset_cmplt_t)(void);
typedef void (*mcp2515_rts_cmplt_t)(void);

bool mcp2515_write(uint8_t* tx_data, uint8_t address, uint8_t length,
                   mcp2515_write_cmplt_t write_cmplt_cbk);
bool mcp2515_read(uint8_t address, uint8_t length, mcp2515_read_cmplt_t read_cmplt_cbk);
bool mcp2515_bit_modify(uint8_t port, uint8_t bit_mask, uint8_t data,
                        mcp2515_bit_modify_cmplt_t bit_modify_cmplt_cbk);
bool mcp2515_read_status(mcp2515_read_status_cmplt_t bit_modify_cmplt_cbk);
bool mcp2515_reset(mcp2515_reset_cmplt_t reset_cmplt_cbk);
bool mcp2515_request_to_send(uint8_t address, mcp2515_rts_cmplt_t rts_cmplt_cbk);
