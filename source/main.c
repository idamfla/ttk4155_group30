#include <stdio.h>

#include "usart/printf.h"
#include "xmem/xmem.h"

#define FOSC  4915200  // Clock Speed
#define BAUD  38400    // Baud rate
#define UBRR0 (FOSC / 16 / BAUD - 1)

int main(void) {
    printf_init(USART0, UBRR0);
    xmem_init();
    return 0;
}
