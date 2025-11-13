/**
 * @file quad_encoder.c
 * @author Marius Bär
 * @brief Initialize and read positions value of the rotary quadrature encoder
 * @date 2025-11-11
 */

#include "quad_encoder.h"

#include "sam.h"

/**
 * @brief Initialize Timer Counter 2 (TC2) for quadrature decoding.
 */
void tc2_qdec_init(void) {
    // Enable the peripheral clock for TC2
    PMC->PMC_PCER1 = (1U << (ID_TC6 - 32));  // TC2 channel 0 (TC6 global ID)

    // Disable TC clock before configuration
    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKDIS;

    // Configure pins for quadrature input
    PIOC->PIO_PDR = PIO_PDR_P25 | PIO_PDR_P26;      // Disable PIO, enable peripheral
    PIOC->PIO_ABSR |= PIO_ABSR_P25 | PIO_ABSR_P26;  // Select peripheral Bs

    // Configure the Block Mode Register for quadrature decoding
    TC2->TC_BMR = TC_BMR_QDEN       // Enable QDEC mode
                  | TC_BMR_POSEN    // Enable position measurement
                  | TC_BMR_EDGPHA;  // Use edges on PHA

    // Configure channel mode register
    TC2->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_XC0;

    // Enable the clock for TC2 channel 0
    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

/**
 * @brief Get the current position from the quadrature encoder.
 * @brief The range is roughly 5600 increments from endstop to endstop.
 * @return int32_t Current position value.
 */
int32_t tc2_qdec_get_position(void) {
    return (int32_t)TC2->TC_CHANNEL[0].TC_CV;
}

/**
 * @brief Reset the quadrature encoder position to zero.
 */
void tc2_qdec_reset(void) {
    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_SWTRG;
}
