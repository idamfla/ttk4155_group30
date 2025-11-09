/**
 * @file ir.h
 * @author Marius Bär
 * @brief Header file for ir.c
 * @date 2025-11-08
 */

#pragma once

#include <stdint.h>

void ir_init(void);
uint16_t ir_read(void);
