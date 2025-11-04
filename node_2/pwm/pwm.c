/**
 * @file pwm.c
 * @author author
 * @brief brief
 * @date 2025-11-04
 */

#include "pwm.h"

#include "sam.h"

#define PA8_MASK (1U << 8)

void pwm_init() {
    // REG_PMC_PCER1 = (1 << (ID_PWM - 32));

    REG_PMC_PCER1 = PMC_PCDR1_PID36;

    *((volatile unsigned int*)(PIOA + PIO_PDR_P2)) = PA8_MASK;
    *((volatile unsigned int*)(PIOA + 0x70)) = PA8_MASK;
}