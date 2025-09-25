// Has to be defined before the inclusion of delay.h
#define F_CPU 4915200  // Clock Speed

#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>

#include "max156/max156.h"
#include "spi/spi.h"
#include "usart/printf.h"
#include "xmem/xmem.h"

#define BAUD  38400  // Baud rate
#define UBRR0 (F_CPU / 16 / BAUD - 1)

uint8_t test_data[] = {'N', 'T', 'N', 'U'};

int main(void) {
    printf_init(USART0, UBRR0);
    xmem_init();
    sei();

    spi_master_init();
    const SPITransferData test = {
        .data = test_data, .length = sizeof(test_data), .recieved = NULL, .ss = SS1};
    while (1) {
        spi_start_data_transfer(&test);
        _delay_ms(100);
    }
    return 0;
}
