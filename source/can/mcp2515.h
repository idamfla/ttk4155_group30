/**
 * @file mcp2515.h
 * @author author
 * @brief Header file for mcp2515.c
 * @date 2025-10-09
 */

#pragma once
#include <stdbool.h>
#include <stdint.h>

#ifndef CAN_H
#    define CAN_H

#endif /* CAN_H */

void mcp2515_init();
bool mcp2515_bit_modify(uint8_t port, uint8_t bit_mask, uint8_t data);
bool mcp2515_write(uint8_t tx_data, uint8_t address, uint8_t length);
bool mcp2515_read(uint8_t* rx_data, uint8_t address);
bool mcp2515_read_status(uint8_t* rx_data);
void mcp2515_reset(void);