/**
 * @file timer_counter.c
 * @author author
 * @brief brief
 * @date 2025-11-09
 */

#include "timer_counter.h"

#define TC_WPKEY 0x54494D

/**
 * @brief init for the tc0
 * @param ch_mask masks which channels you want to enabel. The ids are `27`, `28` and `29`
 * @param period the period of the wave
 * @note `(1U << ID_TC0)` to enable channel 0
 * @note `(1U << ID_TC1)` to enable channel 1
 * @note `(1U << ID_TC2)` to enable channel 2
 */
void tc0_init(uint32_t ch_mask, uint8_t period) {
    REG_TC0_WPMR |= TC_WPMR_WPKEY(TC_WPKEY) | TC_WPMR_WPEN;

    if (ch_mask & (1U << ID_TC0)) {
        REG_PMC_PCER0 |= PMC_PCER0_PID27;  // enable clock, TC0

        REG_PIOB_PDR |= PIO_PB27;   // enables peripheral control of PB27, TIOB0
        REG_PIOB_ABSR |= PIO_PB27;  // select Peripheral B function for PB27, TIOB0
        _tc0_channel_init(0, period);
        NVIC_EnableIRQ(TC0_IRQn);  // enables that IRQ line in the NVIC
    }

    if (ch_mask & (1U << ID_TC1)) {
        REG_PMC_PCER0 |= PMC_PCER0_PID28;  // enable clock, TC1

        REG_PIOA_PDR |= PIO_PA2;    // enables peripheral control of PA2, TIOB0
        REG_PIOA_ABSR &= ~PIO_PA2;  // select Peripheral A function for PA2, TIOB0
        _tc0_channel_init(1, period);
        NVIC_EnableIRQ(TC1_IRQn);  // enables that IRQ line in the NVIC
    }

    if (ch_mask & (1 << ID_TC2)) {
        REG_PMC_PCER0 |= PMC_PCER0_PID29;  // enable clock, TC2

        REG_PIOA_PDR |= PIO_PA5;    // enables peripheral control of PA5, TIOA2
        REG_PIOA_ABSR &= ~PIO_PA5;  // select Peripheral A function for PA5, TIOA2
        _tc0_channel_init(2, period);
        NVIC_EnableIRQ(TC2_IRQn);  // enable interruptsRQ line in the NVIC
    }
}

void _tc0_channel_init(uint8_t ch, uint8_t period) {
    TcChannel* tc0 = &TC0->TC_CHANNEL[ch];

    tc0->TC_CMR |= TC_CMR_WAVE | TC_CMR_TCCLKS_TIMER_CLOCK4 | TC_CMR_WAVSEL_UP_RC |
                   TC_CMR_ACPA_SET |
                   TC_CMR_ACPC_CLEAR;  // enable waveform ode, select timer clock4 (/128), count to
                                       // RC and then reset, set output high when count is equal to
                                       // RA, clear output low when count is equal to RC
    tc0->TC_RA = period / 2;           // compare on RA
    tc0->TC_RC = period;               // compare on RC
    tc0->TC_CCR |= TC_CCR_CLKEN | TC_CCR_SWTRG;  // enable clock for this channel, soft trigger:
                                                 // start counting immediate from 0
    tc0->TC_IER |= TC_IER_CPCS;                  // enable interrupt on RC compare match
}

void TC0_Handler(void) {
    NVIC_ClearPendingIRQ(TC0_IRQn);
}

void TC1_Handler(void) {
    // TODO think the PI should be updated inside here, where should PI_init be called? also where
    // is the values that the PI holds being used? what is the meaning of PI if you dont use the
    // values that it holds?
    pi_update();
    NVIC_ClearPendingIRQ(TC1_IRQn);
}