/**
 * @file timer_counter.h
 * @author author
 * @brief Header file for timer_counter.c
 * @date 2025-11-09
 */

#pragma once

#include "sam.h"

// clang-format off

// clang-format on

void tc0_init(uint8_t ch_mask, uint8_t period);
void _tc0_channel_init(uint8_t ch, uint8_t period);
