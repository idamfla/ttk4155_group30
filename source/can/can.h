/**
 * @file can.h
 * @author author
 * @brief Header file for can.c
 * @date 2025-10-23
 */

#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint16_t id;
    uint8_t data;
    uint8_t length;
} CAN_DATA;

void CAN_init();
void CAN_send(CAN_DATA* can_data);
bool CAN_recieve_msg(uint8_t* rx_data, uint8_t address);