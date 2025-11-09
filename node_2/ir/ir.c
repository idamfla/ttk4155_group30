/**
 * @file ir.c
 * @author Marius Bär
 * @brief Initialize and read analog infrared sensor values
 * @date 2025-11-08
 */

#include "ir.h"

#include "sam.h"

/**
 * @brief Initialize the infrared sensor ADC
 */
void ir_init(void) {
    // Enable the peripheral clock for ADC
    PMC->PMC_PCER0 |= (1 << ID_PIOA);          // enable PIOA clock (for ADC pin)
    PMC->PMC_PCER1 |= (1U << (ID_ADC - 32U));  // enable clock for peripheral with ID 37

    // Configure the pin for ADC input (assuming PA16 is used for the IR sensor)
    PIOA->PIO_PDR = PIO_PA16;  // disable PIO control of PA16
    PIOA->PIO_ODR = PIO_PA16;  // disable output (set as input)

    // Configure the ADC
    ADC->ADC_MR = ADC_MR_FREERUN_ON       // Free running mode
                  | ADC_MR_PRESCAL(20)    // Set prescaler to divide by 21
                  | ADC_MR_STARTUP_SUT64  // Startup time
                  | ADC_MR_TRACKTIM(3)    // Tracking time
                  | ADC_MR_TRGEN_DIS;     // Disable hardware trigger

    // Enable the channel for the IR sensor (AD0 on the Arduino Due corresponds to channel 7)
    ADC->ADC_CHER = ADC_CHER_CH7;
    // Start conversion
    ADC->ADC_CR = ADC_CR_START;
}

/**
 * @brief Read the value from the infrared sensor
 * @return The converted value from the IR sensor (from 0 to 4095)
 */
uint16_t ir_read(void) {
    // Read and return the converted value from channel 7
    return ADC->ADC_CDR[7];
}
