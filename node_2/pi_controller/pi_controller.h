/**
 * @file pi_controller.h
 * @author author
 * @brief Header file for pi_controller.c
 * @date 2025-11-09
 */

#pragma once
#include <stdint.h>

#include "sam.h"

typedef struct {
    int32_t Kp;
    int32_t Ki;
    volatile int32_t err_integral;
    int32_t out_min;
    int32_t out_max;
} pi_t;

void pi_init(pi_t* me, int32_t kp, int32_t ki, int32_t T, int32_t output_min, int32_t output_max);
int32_t pi_update(pi_t* me, int32_t reference, int32_t measurement);
