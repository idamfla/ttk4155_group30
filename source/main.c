#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>

#include "usart/usart.h"

#define FOSC  4915200  // Clock Speed
#define BAUD  9600     // Baud rate
#define UBRR0 (FOSC / 16 / BAUD - 1)

void usart0_rxcbk(uint8_t data);

int fdef_put(char c, FILE *f);
int fdef_get(FILE *f);

int counter = 0;

int main(void) {
    sei();
    fdevopen(fdef_put, fdef_get);
    usart_init_interrupt(USART0, UBRR0, usart0_rxcbk);
    while (1) {
        printf("Counter: %d\n\r", counter++);
    }

    return 0;
}

int fdef_put(char c, FILE *f) {
    usart_transmit(USART0, c);
    (void)f;
    return 0;
}
int fdef_get(FILE *f) {
    (void)f;
    return 0;
}

void usart0_rxcbk(uint8_t data) {
    usart_transmit(USART0, data);
}