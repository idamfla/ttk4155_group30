/**
 * @file usart.c
 * @author Marius Bär
 * @brief USART driver for the ATmega162
 * @date 2025-09-08
 */

#include "usart.h"

#include <avr/io.h>

/* UCSRA bit definitions */
#define RXC  RXC0
#define TXC  TXC0
#define UDRE UDRE0
#define FE   FE0
#define DOR  DOR0
#define UPE  UPE0
#define U2X  U2X0
#define MPCM MPCM0

/* UCSRB bit definitions */
#define RXCIE RXCIE0
#define TXCIE TXCIE0
#define UDRIE UDRIE0
#define RXEN  RXEN0
#define TXEN  TXEN0
#define UCSZ2 UCSZ02
#define RXB8  RXB80
#define TXB8  TXB80

/* UCSRC bit definitions */
#define URSEL URSEL0
#define UMSEL UMSEL0
#define UPM1  UPM01
#define UPM0  UPM00
#define USBS  USBS0
#define UCSZ1 UCSZ01
#define UCSZ0 UCSZ00
#define UCPOL UCPOL0

/**
 * @brief Base function to initialize USART0 or USART1 in asynchronous mode with 8 data bits and 2
 * stop bits.
 * @param usart Pointer to the usart struct.
 * @param ubrr The value to be set in the UBRR register to configure the baud rate.
 * @note The baud rate can be calculated using the formula (see datasheet page 169):
 * \f$ UBRR = \frac{F_{\mathrm{OSC}}}{16 \cdot \mathrm{Baud}} - 1 \f$
 */
void usart_init(volatile usart_t *usart, uint16_t ubrr) {
    /* Set baud rate */
    volatile uint8_t *ubrrh_c;

    if (usart == USART0) {
        ubrrh_c = &UBRR0H;
    } else {
        ubrrh_c = &UBRR1H;
    }
    *ubrrh_c = (uint8_t)(ubrr >> 8);
    usart->ubrrl = (uint8_t)ubrr;
    /* Enable receiver and transmitter */
    usart->ucsrb = (1 << RXEN) | (1 << TXEN);
    /* Set frame format: 8data, 2stop bit */
    *ubrrh_c = (1 << USBS) | (3 << UCSZ0);
}

/**
 * @brief Check if the USART transmit buffer is empty.
 * @param usart Pointer to the usart struct.
 */
bool usart_tx_ready(volatile usart_t *usart) {
    return (usart->ucsra & (1 << UDRE));
}

/**
 * @brief Transmit a single byte of data via USART.
 * @param usart Pointer to the usart struct.
 * @param data The byte of data to be transmitted.
 */
void usart_transmit(volatile usart_t *usart, uint8_t data) {
    /* Wait for empty transmit buffer */
    while (!usart_tx_ready(usart));
    /* Put data into buffer, sends the data */
    usart->udr = data;
}
