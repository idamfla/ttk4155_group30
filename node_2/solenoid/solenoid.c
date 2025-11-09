/**
 * @file solenoid.c
 * @author Marius Bär
 * @brief Solenoid control
 * @date 2025-11-09
 */

#include "solenoid.h"

#include "sam.h"

void solenoid_init(void) {
    // Enable the peripheral clock for PIOA
    PMC->PMC_PCER0 = (1U << ID_PIOA);

    // Disable PIOA’s peripheral control on PA29 to use it as a GPIO
    PIOA->PIO_PER = PIO_PA6;

    // Set PA29 as an output
    PIOA->PIO_OER = PIO_PA6;

    // Set PA29 to high
    PIOA->PIO_SODR = PIO_PA6;
}

void solenoid_set_state(bool state) {
    if (state) {
        // Clear PA29
        PIOA->PIO_CODR = PIO_PA6;
    } else {
        // Set PA29
        PIOA->PIO_SODR = PIO_PA6;
    }
}
