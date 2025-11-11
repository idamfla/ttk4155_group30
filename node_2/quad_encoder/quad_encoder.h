/**
 * @file quad_encoder.h
 * @author Marius Bär
 * @brief Header file for quad_encoder.c
 * @date 2025-11-11
 */

#pragma once

/**
 * @file quad_encoder.c
 * @author Marius Bär
 * @brief Initialize and read positions value of the rotary quadrature encoder
 * @date 2025-11-11
 */

#include "quad_encoder.h"
#include "sam.h"

void tc2_qdec_init(void);
int32_t tc2_qdec_get_position(void);
void tc2_qdec_reset(void);
