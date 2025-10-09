/**
 * @file can.h
 * @author author
 * @brief Header file for can.c
 * @date 2025-10-09
 */

#pragma once
#include <stdint.h>

#ifndef CAN_H
#    define CAN_H

#endif /* CAN_H */

void mcp2515_init();
uint8_t mcp2515_read(uint8_t address);
void mcp2515_reset(void);