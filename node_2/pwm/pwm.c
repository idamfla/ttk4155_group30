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
#define F_PWM             20  // 20 ms = 50 Hz
#define F_PWM_CLK         21  // 21 ms, WHY???
#define CPRD              (F_PWM_CLK / F_PWM)
#define DUTY_CYCLE_MIDDLE 500   // 1.5 ms, middle value
#define SERVO_UPPER_LIMIT 1112  // 2.1 ms, lower value
#define SERVO_LOWER_LIMIT 500   // 0.9 ms, lower value

/**
 * @brief set pin in pmc register
 * @param reg the register, has to be defined like this: `(*(volatile unsigned int*)address)`
 * @param pin the pin, has to be defined like: `(0x1U << pin_number)`
 */
#define SET_PIN_SAM3(reg, pin) (reg |= pin);

/**
 * @brief clear pin in pmc register
 * @param reg the register, has to be defined like this: `(*(volatile unsigned int*)address)`
 * @param pin the pin, should be defined like: `(0x1U << pin_number)`
 */
#define CLEAR_PIN_SAM3(reg, pin) (reg &= ~(pin));

void pwm_init(void) {
    // ----- CLOCK -----
    CLEAR_PIN_SAM3(REG_PMC_WPMR, PMC_WPMR_WPEN);        // disable write protect PMC
    SET_PIN_SAM3(REG_PMC_MCKR, PMC_MCKR_CSS_MAIN_CLK);  // might be unnecessary
    SET_PIN_SAM3(REG_PMC_PCER1,
                 (1 << (ID_PWM - 32)));         // enable the clock for the pwm peripheral
    SET_PIN_SAM3(REG_PMC_WPMR, PMC_WPMR_WPEN);  // enable write protect PMC

    // ----- SHIELDS ----- // I think
    CLEAR_PIN_SAM3(REG_PIOC_WPMR, PIO_WPMR_WPEN);  // disable write protect PIO
    SET_PIN_SAM3(REG_PIOC_PDR, PIO_PDR_P3);        // might be wrong, maybe p2
    SET_PIN_SAM3(REG_PIOC_ABSR, PIO_ABSR_P3);      // might be wrong, maybe p2
    SET_PIN_SAM3(REG_PIOC_WPMR, PIO_WPMR_WPEN);    // enable write protect PIO

    // ----- CHANNEL MODE -----
    CLEAR_PIN_SAM3(REG_PWM_WPCR,
                   PWM_WPCR_WPCMD);  // disable write protect PWM, TODO why does PWM_WPCR_WPCMD take
                                     // a parameter, and what to set it to???
    SET_PIN_SAM3(
        REG_PWM_CLK,
        PWM_CLK_PREA(1) | PWM_CLK_DIVA(2));  // TODO find out what it does ... something about clock
    SET_PIN_SAM3(REG_PWM_CMR0, PWM_CMR_CPRE_CLKA);
    SET_PIN_SAM3(REG_PWM_CPRD0, CPRD);               // sets periode
    SET_PIN_SAM3(REG_PWM_CDTY0, DUTY_CYCLE_MIDDLE);  // sets duty cycle
    SET_PIN_SAM3(REG_PWM_WPCR,
                 PWM_WPCR_WPCMD);  // enable write protect PWM, TODO why does PWM_WPCR_WPCMD take a
                                   // parameter, and what to set it to???

    SET_PIN_SAM3(REG_PWM_ENA, PWM_ENA_CHID0);  // enable pwm output for channel 0
}

/**
 * @brief makes sure that the pwm never exceeds the bounderies of the servo
 * @param duty_cycle the duty_cycle, it will be saturated if it goes out of bound
 */
void pwm_duty_cycle_guard(uint32_t duty_cycle) {
    if ((duty_cycle < SERVO_LOWER_LIMIT) || (duty_cycle > SERVO_UPPER_LIMIT)) return;

    if (duty_cycle < SERVO_LOWER_LIMIT) {
        duty_cycle = SERVO_LOWER_LIMIT;
    } else if (duty_cycle > SERVO_UPPER_LIMIT) {
        duty_cycle = SERVO_UPPER_LIMIT;
    }

    SET_PIN_SAM3(REG_PWM_CDTYUPD0, duty_cycle);
}