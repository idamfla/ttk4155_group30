/**
 * @file pwm.h
 * @author author
 * @brief Header file for pwm.c
 * @date 2025-11-04
 */

#pragma once
#include <stdint.h>

// clang-format off

// clang-format on

void pwm_init(void);
void pwm_duty_cycle_guard(uint32_t duty_cycle);