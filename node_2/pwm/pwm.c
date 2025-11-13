/**
 * @file pwm.c
 * @author author
 * @brief brief
 * @date 2025-11-04
 */

#include "pwm.h"

#include "sam.h"

#define DIVA 26U
#define DIVB 1U

// clang-format off
/**
 * @brief disable and enable write protect for the PWM
 * @param value either `0` or `1`, which translate to disable or enable
 */
#define PWM_WP_ENABLE(value)                                                     \
    (REG_PWM_WPCR = (PWM_WPCR_WPKEY(0x50574D) |                                  \
                     PWM_WPCR_WPRG0 | PWM_WPCR_WPRG1 | PWM_WPCR_WPRG2 |          \
                     PWM_WPCR_WPRG3 | PWM_WPCR_WPRG4 | PWM_WPCR_WPRG5 |          \
                     PWM_WPCR_WPCMD(value)))
// clang-format on

void pwm_init(void) {
    // ----- CLOCK -----
    REG_PMC_PCER0 |= (1 << ID_PIOB);        // enable clock for peripheral with ID 12
    REG_PMC_PCER1 |= (1 << (ID_PWM - 32));  // enable clock for peripheral with ID 36

    // ----- PIO CONFIGURATION -----
    REG_PIOB_PDR = PIO_PDR_P12;     // enables peripheral control of PB12
    REG_PIOB_ABSR |= PIO_ABSR_P12;  // select Peripheral A function for PB12
    REG_PIOB_MDDR = PIO_MDDR_P12;   // ensures the peripheral can drive the pin fully

    REG_PIOB_PDR = PIO_PDR_P13;     // enables peripheral control of PB13
    REG_PIOB_ABSR |= PIO_ABSR_P13;  // select Peripheral B function for PB13
    REG_PIOB_MDDR = PIO_MDDR_P13;   // ensures the peripheral can drive the pin fully

    if ((REG_PWM_WPSR & PWM_WPSR_WPHWS0) | (REG_PWM_WPSR & PWM_WPSR_WPHWS1)) {
        printf("WP fault, CH0 or CH1 are protected\r\n");
        return;
    }

    // ----- CLOCK -----
    REG_PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(DIVA) | PWM_CLK_DIVB(DIVB);  // set clk div

    // ----- CONFIG CHANNEL 0 -----
    REG_PWM_CMR0 = PWM_CMR_CPRE_CLKB | PWM_CMR_CPOL;
    REG_PWM_CPRD0 = CPRD0;         // sets periode
    REG_PWM_CDTY0 = 0U;            // sets duty cycle, 0%
    REG_PWM_ENA |= PWM_ENA_CHID0;  // enable pwm output for channel 0

    // ----- CONFIG CHANNEL 1 -----
    REG_PWM_CMR1 = PWM_CMR_CPRE_CLKA | PWM_CMR_CPOL;
    REG_PWM_CPRD1 = CPRD1;         // sets periode
    REG_PWM_CDTY1 = CDTY1_MIDDLE;  // sets duty cycle, 50%
    REG_PWM_ENA |= PWM_ENA_CHID1;  // enable pwm output for channel 1
}

/**
 * @brief Set the duty cycle of the PWM for the servo
 * @param duty_cycle Duty cycle value between CDTY_MIN and CDTY_MAX
 */
void pwm_set_dc_servo(uint32_t duty_cycle) {
    if (duty_cycle < CDTY1_MIN) {
        duty_cycle = CDTY1_MIN;
    } else if (duty_cycle > CDTY1_MAX) {
        duty_cycle = CDTY1_MAX;
    }

    REG_PWM_CDTYUPD1 = duty_cycle;
}

/**
 * @brief Set the duty cycle of the PWM for the motor
 * @param duty_cycle Duty cycle value between CDTY_MIN and CDTY_MAX
 */
void pwm_set_dc_motor(uint32_t duty_cycle) {
    if (duty_cycle > CPRD0) {
        duty_cycle = CPRD0;
    }
    REG_PWM_CDTYUPD0 = duty_cycle;
}