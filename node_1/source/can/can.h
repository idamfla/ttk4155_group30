/**
 * @file can.h
 * @author author
 * @brief Header file for can.c
 * @date 2025-10-23
 */

#pragma once
#include <stdint.h>

typedef struct
{
    uint8_t id;
    uint8_t address;
    uint8_t data;
    uint8_t length;
} CAN_DATA;

void CAN_send(CAN_DATA *can_data);
CAN_DATA CAN_recieve_msg(uint8_t *rx_data, uint8_t address);