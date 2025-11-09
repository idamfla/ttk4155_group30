/**
 * @file pwm.c
 * @author author
 * @brief brief
 * @date 2025-11-04
 */

#include "pwm.h"

#include "sam.h"

#define DIVA 26U
#define CPRD 64612U  // period

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
    REG_PIOB_PDR = PIO_PDR_P13;    // enables peripheral control of PB13
    REG_PIOB_ABSR = PIO_ABSR_P13;  // peripheral B function of PID12 to PB13
    REG_PIOB_MDDR = PIO_MDDR_P13;  // ensures the peripheral can drive the pin fully

    PWM_WP_ENABLE(0);  // disable pwm wp
    if ((REG_PWM_WPSR & PWM_WPSR_WPHWS0) | (REG_PWM_WPSR & PWM_WPSR_WPHWS1)) {
        printf("WP fault, CH0 or CH1 are protected\r\n");
        return;
    }

    // ----- CLOCK -----
    REG_PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(DIVA);  // set clk div

    // ----- CHANNEL MODE -----
    REG_PWM_CMR1 = PWM_CMR_CPRE_CLKA | PWM_CMR_CPOL;
    REG_PWM_CPRD1 = CPRD;         // sets periode
    REG_PWM_CDTY1 = CDTY_MIDDLE;  // sets duty cycle, 50%
    REG_PWM_ENA = PWM_ENA_CHID1;  // enable pwm output for channel 0

    PWM_WP_ENABLE(1);  // enable pwm wp
}

/**
 * @brief Set the duty cycle of the PWM
 * @param duty_cycle Duty cycle value between CDTY_MIN and CDTY_MAX
 */
void pwm_set_dc(uint32_t duty_cycle) {
    if (duty_cycle < CDTY_MIN) {
        duty_cycle = CDTY_MIN;
    } else if (duty_cycle > CDTY_MAX) {
        duty_cycle = CDTY_MAX;
    }

    REG_PWM_CDTYUPD1 = duty_cycle;
}