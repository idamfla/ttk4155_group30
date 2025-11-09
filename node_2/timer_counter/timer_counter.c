/**
 * @file timer_counter.c
 * @author author
 * @brief brief
 * @date 2025-11-09
 */

#include "timer_counter.h"

void tc_init() {
    REG_PMC_PCER0 |= PMC_PCER0_PID27;  // enable clock, tc0

    REG_PIOB_PDR |= PIO_PB27;   // enables peripheral control of PB27, TIOB0
    REG_PIOB_ABSR |= PIO_PB27;  // select Peripheral B function for PB27, TIOB0
}

void TC0_Handler(void) {}