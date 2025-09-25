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
uint8_t test_data2[] = {'T', 'T', 'K', '4', '1', '5', '5'};

int main(void) {
    printf_init(USART0, UBRR0);
    xmem_init();
    sei();

    spi_master_init();
    const spi_transfer_t test = {.tx_data = test_data,
                                 .rx_data = NULL,
                                 .length = sizeof(test_data),
                                 .slave_idx = spi_slave_disp_c};
    const spi_transfer_t test2 = {.tx_data = test_data2,
                                  .rx_data = NULL,
                                  .length = sizeof(test_data2),
                                  .slave_idx = spi_slave_io};
    while (1) {
        spi_transfer(&test);
        spi_transfer(&test2);
        _delay_ms(1000);
    }
    return 0;
}
