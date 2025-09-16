#include <stdio.h>

#include "usart/printf.h"

#define FOSC  4915200  // Clock Speed
#define BAUD  38400    // Baud rate
#define UBRR0 (FOSC / 16 / BAUD - 1)

int main(void) {
    printf_init(USART0, UBRR0);

    // Configure external memory high mask to only use four lines of the high byte of the address.
    // This release the pins PC4 to PC7 (for JTAG).
    SFIOR = (1 << XMM2);
    // Enable external SRAM.
    MCUCR = (1 << SRE);

    while (1) {
        uint8_t *p;
        p = (uint8_t *)0x1FFF;
        *p = 1U;
    }

    return 0;
}
