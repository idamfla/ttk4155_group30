/**
 * @file timer.c
 * @author Marius Bär
 * @brief Configure timer for periodic execution
 * @date 2025-10-14
 */

#include "timer.h"

#include <avr/io.h>

#include "../constants.h"

void timer1_init(uint8_t rate) {
    TCCR1B |= (1 << CS10) | (1 << CS11);            // Prescaler = 64
    TCCR1B |= (1 << WGM12);                         // Set WGM12 bit for CTC mode (Mode 4)
    OCR1A = F_CPU / 64UL / ((uint32_t)rate) - 1UL;  // Set compare value for desired frequency
    TIMSK |= (1 << OCIE1A);                         // Enable Timer1 Compare A interrupt
}
