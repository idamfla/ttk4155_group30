/**
 * @file can.h
 * @author Marius Bär
 * @brief Header file for can.c
 * @date 2025-11-16
 */

#pragma once

#include <stdbool.h>

typedef enum {
    can_event_none,
    can_event_mcp2515_reset_done,
    can_event_mcp2515_bit_modify_done,
    can_event_mcp2515_write_done,
    can_event_tx_start,
} can_event_t;

typedef enum {
    can_state_wait_for_reset,
    can_state_wait_config_mode,
    can_state_wait_write_config,
    can_state_wait_normal_mode,
    can_state_idle,
    can_state_transmitting,
} can_state_t;

bool can_init(void);
void can_update(can_event_t event);
can_state_t can_get_state(void);
