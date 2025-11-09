/**
 * @file pi_controller.h
 * @author author
 * @brief Header file for pi_controller.c
 * @date 2025-11-09
 */

#pragma once
#include <stdint.h>

#include "sam.h"

// clang-format off

// clang-format on

typedef struct {
    uint8_t Kp;
    uint8_t Ki;
    uint8_t integrator;
    uint8_t past_errors;
    uint8_t T;
    uint8_t out_min;
    uint8_t out_max;
} PI;

void pi_init(uint8_t kp, uint8_t ki, uint8_t output_min, uint8_t output_max);
uint8_t pi_update(uint8_t reference, uint8_t measurement);