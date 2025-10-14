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

void can_init();
uint8_t can_read(uint8_t address);
void can_reset(void);