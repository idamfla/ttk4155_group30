/**
 * @file pwm.c
 * @author author
 * @brief brief
 * @date 2025-11-04
 */

#include "pwm.h"

#include "sam.h"

// 125 - goes, freq
// 250 - might go, freq
// 220kohm - slew rate

// TODO Make sure that all the values are the right format
#define DIVA        26U
#define CPRD        64615U
#define CDTY_MIN    2908  // 0.9 ms, lower value
#define CDTY_MIDDLE 4846  // 1.5 ms, middle value
#define CDTY_MAX    6784  // 2.1 ms, lower value

/**
 * @brief clear pin in pmc register
 * @param reg the register, has to be defined like this: `(*(volatile unsigned int*)address)`
 * @param pin the pin, should be defined like: `(0x1U << pin_number)`
 */
#define CLEAR_PIN_SAM3(reg, pin) (reg &= ~(pin));

void pwm_init(void) {
    // ----- CLOCK -----
    CLEAR_PIN_SAM3(REG_PMC_WPMR, PMC_WPMR_WPEN);  // disable write protect PMC
    REG_PMC_PCER1 = (1 << (ID_PWM - 32));         // enable the clock for the pwm peripheral
    REG_PMC_WPMR = PMC_WPMR_WPEN;                 // enable write protect PMC

    // ----- SHIELDS ----- // I think
    // CLEAR_PIN_SAM3(REG_PIOC_WPMR, PIO_WPMR_WPEN);  // disable write protect PIO
    REG_PIOC_PDR = PIO_PDR_P3;    // might be wrong, maybe p2
    REG_PIOC_ABSR = PIO_ABSR_P3;  // might be wrong, maybe p2
    // REG_PIOC_WPMR = PIO_WPMR_WPEN;    // enable write protect PIO

    // ----- CHANNEL MODE -----
    REG_PWM_WPCR = PWM_WPCR_WPKEY(0x50574D) | PWM_WPCR_WPRG0 | PWM_WPCR_WPRG1 | PWM_WPCR_WPRG2 |
                   PWM_WPCR_WPRG3 | PWM_WPCR_WPRG4 | PWM_WPCR_WPRG5 | PWM_WPCR_WPCMD(0);

    REG_PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(DIVA);  // set clk div
    // SET_PIN_SAM3(REG_PWM_CMR0, PWM_CMR_CPRE_CLKA);
    REG_PWM_CMR0 = PWM_CMR_CPRE_CLKA | PWM_CMR_CPOL;
    REG_PWM_CPRD0, CPRD;         // sets periode
    REG_PWM_CDTY0, CDTY_MIDDLE;  // sets duty cycle
    REG_PWM_WPCR = PWM_WPCR_WPKEY(0x50574D) | PWM_WPCR_WPRG0 | PWM_WPCR_WPRG1 | PWM_WPCR_WPRG2 |
                   PWM_WPCR_WPRG3 | PWM_WPCR_WPRG4 | PWM_WPCR_WPRG5 | PWM_WPCR_WPCMD(1);

    REG_PWM_ENA, PWM_ENA_CHID0;  // enable pwm output for channel 0
}

/**
 * @brief makes sure that the pwm never exceeds the bounderies of the servo
 * @param duty_cycle the duty_cycle, it will be saturated if it goes out of bound
 */
void pwm_duty_cycle_guard(uint32_t duty_cycle) {
    if (duty_cycle < CDTY_MIN) {
        duty_cycle = CDTY_MIN;
    } else if (duty_cycle > CDTY_MAX) {
        duty_cycle = CDTY_MAX;
    }

    REG_PWM_CDTYUPD0 = duty_cycle;
}