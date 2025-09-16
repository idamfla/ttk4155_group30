#include <stdio.h>

#include "usart/printf.h"

#define FOSC  4915200  // Clock Speed
#define BAUD  38400    // Baud rate
#define UBRR0 (FOSC / 16 / BAUD - 1)

int main(void) {
    printf_init(USART0, UBRR0);
    while (1) {
        uint8_t *p;
        p = (uint8_t *)0x1FFF;
        *p = 1U;
    }

    return 0;
}
