/**
 * @file solenoid.h
 * @author Marius Bär
 * @brief Header file for solenoid.c
 * @date 2025-11-09
 */

#pragma once

#include <stdbool.h>

void solenoid_init(void);
void solenoid_set_state(bool state);
