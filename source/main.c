#include <stdio.h>

#include "usart/printf.h"

#define FOSC  4915200  // Clock Speed
#define BAUD  9600     // Baud rate
#define UBRR0 (FOSC / 16 / BAUD - 1)

int counter = 0;

int main(void) {
    printf_init(USART0, UBRR0);
    while (1) {
        printf("Counter: %d\n\r", counter++);
    }

    return 0;
}
