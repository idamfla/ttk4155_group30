/**
 * @file timer.c
 * @author Marius Bär
 * @brief Init timer peripheral
 * @date 2025-11-13
 */

#include "timer.h"

#include "constants.h"
#include "sam.h"

#define CLOCK_DIVIDER 2
#define MS_PER_SEC    (1000)

void (*_timer_cbk)(void);

void tc0_init(uint32_t interval_ms, void (*timer_cbk)(void)) {
    // Set the callback function
    _timer_cbk = timer_cbk;

    // Enable peripheral clock for TC0 channel 0
    PMC->PMC_PCER0 |= (1 << ID_TC0);

    // Disable the clock while configuring
    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKDIS;

    // Select clock source: MCK/128 and waveform mode, reset on RC compare
    TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK1  // MCK/2
                                | TC_CMR_WAVE               // Waveform mode
                                | TC_CMR_WAVSEL_UP_RC;      // Reset counter on RC compare

    // Compute RC compare value
    uint32_t rc = (F_CPU / MS_PER_SEC / CLOCK_DIVIDER) * interval_ms;

    TC0->TC_CHANNEL[0].TC_RC = rc;

    // Enable interrupt on RC compare
    TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;
    TC0->TC_CHANNEL[0].TC_IDR = ~TC_IER_CPCS;

    // Enable interrupt in NVIC
    NVIC_EnableIRQ(TC0_IRQn);

    // Enable and start the timer
    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

// Interrupt handler
void TC0_Handler(void) {
    // Clear interrupt flag by reading the status register
    uint32_t status = TC0->TC_CHANNEL[0].TC_SR;
    (void)status;  // Prevent unused variable warning

    _timer_cbk();
}
