/**
 * @file pwm.h
 * @author author
 * @brief Header file for pwm.c
 * @date 2025-11-04
 */

#pragma once
#include <stdint.h>
#include <stdio.h>

#include "constants.h"

#define CPRD0 MOTOR_CURRENT_MAX
#define CPRD1 64612U

#define CDTY1_MIN    2909  // 0.9 ms, lower value
#define CDTY1_MIDDLE 4846  // 1.5 ms, middle value
#define CDTY1_MAX    6784  // 2.1 ms, lower value

void pwm_init(void);
void pwm_set_dc_servo(uint32_t duty_cycle);
void pwm_set_dc_motor(uint32_t duty_cycle);
