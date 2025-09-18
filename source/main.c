#include <stdio.h>

#include "max156/max156.h"
#include "usart/printf.h"
#include "xmem/xmem.h"

#define FOSC  4915200  // Clock Speed
#define BAUD  38400    // Baud rate
#define UBRR0 (FOSC / 16 / BAUD - 1)

int main(void) {
    printf_init(USART0, UBRR0);
    xmem_init();

    max156_init();
    max156_data_t data;
    printf("ADC test started...\r\n");

    while (1) {
        max156_trigger_conversion();
        max156_read(&data);
        printf("ADC values: %u, %u, %u, %u\r\n\n\n", data.ch0, data.ch1, data.ch2, data.ch3);
    }
    return 0;
}
