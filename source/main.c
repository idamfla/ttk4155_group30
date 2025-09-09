#include <avr/io.h>

#include "usart/usart.h"

#define FOSC  4915200  // Clock Speed
#define BAUD  19200    // Baud rate
#define UBRR0 (FOSC / 16 / BAUD - 1)

int main(void) {
    usart_init(USART0, UBRR0);
    while (1) {
        usart_transmit(USART0, 'A');
    }

    return 0;
}
