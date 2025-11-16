/**
 * @file can.h
 * @author Marius Bär
 * @brief Header file for can.c
 * @date 2025-11-16
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    can_event_none,
    can_event_mcp2515_reset_done,
    can_event_mcp2515_bit_modify_done,
    can_event_mcp2515_write_done,
    can_event_rts_done,
    can_event_tx_start,
} can_event_t;

typedef enum {
    can_state_wait_for_reset,
    can_state_wait_config_mode,
    can_state_wait_write_config,
    can_state_wait_normal_mode,
    can_state_idle,
    can_state_tx_prepare_data,
    can_state_wait_rts_done,
    can_state_transmitting,
} can_state_t;

typedef struct {
    uint16_t id;
    uint8_t* data;
    uint8_t length;
} can_message_t;

bool can_init(void);
void can_update(can_event_t event);
can_state_t can_get_state(void);
bool can_send(const can_message_t* can_msg);