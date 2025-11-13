/**
 * @file timer.h
 * @author Marius Bär
 * @brief Header file for timer.c
 * @date 2025-11-13
 */

#pragma once

#include <stdint.h>

void tc0_init(uint32_t interval_ms, void (*timer_cbk)(void));
