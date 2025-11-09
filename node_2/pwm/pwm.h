/**
 * @file pwm.h
 * @author author
 * @brief Header file for pwm.c
 * @date 2025-11-04
 */

#pragma once
#include <stdint.h>
#include <stdio.h>

#define CDTY_MIN    2909  // 0.9 ms, lower value
#define CDTY_MIDDLE 4846  // 1.5 ms, middle value
#define CDTY_MAX    6784  // 2.1 ms, lower value

void pwm_init(void);
void pwm_set_dc(uint32_t duty_cycle);