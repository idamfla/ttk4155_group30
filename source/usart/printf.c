/**
 * @file printf.c
 * @author Marius Bär
 * @brief Function to send data over USART using printf
 * @date 2025-09-11
 */

#include "printf.h"

#include <stdio.h>

static volatile usart_t *_printf_usart;

int _put(char c, FILE *f) {
    usart_transmit(_printf_usart, c);
    (void)f;
    return 0;
}

int _get(FILE *f) {
    (void)f;
    return 0;
}

/**
 * @brief Initializes the USART for printf functionality.
 * @param usart Pointer to the usart struct.
 * @param ubrr The value to be set in the UBRR register to configure the baud rate.
 * @see usart_init for baud rate calculation.
 */
void printf_init(volatile usart_t *usart, uint16_t ubrr) {
    usart_init(usart, ubrr);
    fdevopen(_put, _get);
}
