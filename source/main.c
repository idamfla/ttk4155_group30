#include <avr/interrupt.h>
#include <avr/io.h>

#include "usart/usart.h"

#define FOSC  4915200  // Clock Speed
#define BAUD  19200    // Baud rate
#define UBRR0 (FOSC / 16 / BAUD - 1)

void usart0_rxcbk(uint8_t data);

int main(void) {
    sei();
    usart_init_interrupt(USART0, UBRR0, usart0_rxcbk);
    while (1) {
    }

    return 0;
}

void usart0_rxcbk(uint8_t data) {
    usart_transmit(USART0, data);
}