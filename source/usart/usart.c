/**
 * @file usart.c
 * @author Marius Bär
 * @brief USART driver for the ATmega162
 * @date 2025-09-08
 */

#include "usart.h"

#include <avr/io.h>
#include <stdbool.h>

/**
 * @brief Base function to initialize USART0 or USART1 in asynchronous mode with 8 data bits and 2
 * stop bits.
 */
static inline void usart_init(uint16_t ubrr, volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
                              volatile uint8_t *ucsrb, volatile uint8_t *ucsrc, uint8_t rxen,
                              uint8_t txen, uint8_t usbs, uint8_t ucsz0) {
    /* Set baud rate */
    *ubrrh = (uint8_t)(ubrr >> 8);
    *ubrrl = (uint8_t)ubrr;
    /* Enable receiver and transmitter */
    *ucsrb = (1 << rxen) | (1 << txen);
    /* Set frame format: 8data, 2stop bit */
    *ucsrc = (1 << usbs) | (3 << ucsz0);
}

/**
 * @brief Initialize USART0 in asynchronous mode with 8 data bits and 2 stop bits.
 * @param ubrr The value to be set in the UBRR register to configure the baud rate.
 * @note The baud rate can be calculated using the formula (see datasheet page 169):
 * \f$ UBRR = \frac{F_{\mathrm{OSC}}}{16 \cdot \mathrm{Baud}} - 1 \f$
 */
void usart0_init(uint16_t ubrr) {
    usart_init(ubrr, &UBRR0H, &UBRR0L, &UCSR0B, &UCSR0C, RXEN0, TXEN0, USBS0, UCSZ00);
}

/**
 * @brief Initialize USART1 in asynchronous mode with 8 data bits and 2 stop bits.
 * @param ubrr The value to be set in the UBRR register to configure the baud rate.
 * @note The baud rate can be calculated using the formula (see datasheet page 169):
 * \f$ UBRR = \frac{F_{\mathrm{OSC}}}{16 \cdot \mathrm{Baud}} - 1 \f$
 */
void usart1_init(uint16_t ubrr) {
    usart_init(ubrr, &UBRR1H, &UBRR1L, &UCSR1B, &UCSR1C, RXEN1, TXEN1, USBS1, UCSZ10);
}