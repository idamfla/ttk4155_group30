// clang-format off
// Has to be defined before the inclusion of delay.h
#include "constants.h"
// clang-format on

#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>

#include "max156/max156.h"
#include "oled/oled.h"
#include "spi/spi.h"
#include "usart/printf.h"
#include "xmem/xmem.h"

#define BAUD  38400  // Baud rate
#define UBRR0 (F_CPU / 16 / BAUD - 1)

uint8_t test_data[] = {5};
uint8_t test_data2[] = {10};

const spi_transfer_t test = {.tx_data = test_data,
                             .rx_data = NULL,
                             .length = sizeof(test_data),
                             .slave_idx = spi_slave_disp_d,
                             .transfer_cmplt_cbk = NULL};

int main(void) {
    printf_init(USART0, UBRR0);
    xmem_init();
    sei();

    spi_master_init();

    oled_init();

    while (1) {
        _delay_ms(1000);
    }
    return 0;
}
