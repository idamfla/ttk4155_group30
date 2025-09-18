/**
 * @file max156.c
 * @author Marius Bär
 * @brief Driver for the MAX156 ADC
 * @date 2025-09-18
 */

#include "max156.h"

#include <avr/io.h>
#include <stdio.h>

#include "../gpio/gpio.h"
#include "../usart/printf.h"

#define MAX156_BUSY_PORT  PORTB
#define MAX156_BUSY_INPUT PINB
#define MAX156_BUSY_PIN   PB1

#define MAX156_START_ADDR ((volatile uint8_t *)0x1000)

/**
 * @brief Initialize the MAX156 ADC.
 * @note This function must be called before reading from the ADC.
 */
void max156_init(void) {  // Set the Port B pin 0 to an input (used as the BUSY signal).
    DDRB &= ~((uint8_t)(1U) << MAX156_BUSY_PIN);
    // DDRD = 0;
}

/**
 * @brief Read the converted values of all four channels from the ADC.
 * @param data Pointer to a max156_data_t struct where the read values will be stored.
 */
void max156_read(max156_data_t *data) {
    // Wait until the ADC is not busy anymore.
    while (!READ_PIN(MAX156_BUSY_INPUT, MAX156_BUSY_PIN));

    // Read the value from the ADC memory-mapped region.
    // The channel is automatically incremented after each read (see datasheet page).
    data->ch0 = *(MAX156_START_ADDR);
    data->ch1 = *(MAX156_START_ADDR);
    data->ch2 = *(MAX156_START_ADDR);
    data->ch3 = *(MAX156_START_ADDR);
}

/**
 * @brief Trigger a conversion on the ADC.
 * @note This function must be called before reading values from the ADC.
 */
void max156_trigger_conversion(void) {
    // Writing to an address in the ADC memory-mapped region enables the ADC CS and also strobes the
    // WR pin which triggers the conversion.
    // It does not 'write' to the ADC in the general sense.
    *MAX156_START_ADDR = 0;
}
