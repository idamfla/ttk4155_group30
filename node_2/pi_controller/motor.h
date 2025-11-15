/**
 * @file motor.h
 * @author Marius Bär
 * @brief Header file for motor.c
 * @date 2025-11-15
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    int32_t pos_prev;
    int32_t pos_setpoint;  // Used for velocity control with position loop
    int32_t speed_setpoint;
    int32_t current_setpoint;
    int32_t pos_current;
    int32_t speed_current;
} motor_state_t;

void motor_get_state(volatile motor_state_t* state);
int32_t motor_get_current_speed(void);
int32_t motor_get_current_position(void);
void motor_off(void);
void motor_init(int32_t speed_max, int32_t current_max);
void motor_ctrl_pos(int32_t pos_setpoint);
void motor_ctrl_speed(int32_t speed_setpoint, bool with_pos_loop);
