// Has to be defined before the inclusion of delay.h
#define F_CPU 4915200  // Clock Speed

#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>

#include "max156/max156.h"
#include "spi/spi.h"
#include "ssd1309/oled.h"
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
                             .transfer_cmplt_cbk = NULL,
                             .transfer_started = NULL};

int main(void) {
    printf_init(USART0, UBRR0);
    xmem_init();
    sei();

    spi_master_init();
    printf("Spi \n\r");

    oled_init();
    printf("oled \n\r");

    // const spi_transfer_t test2 = {.tx_data = test_data2,
    //                               .rx_data = NULL,
    //                               .length = sizeof(test_data2),
    //                               .slave_idx = spi_slave_disp_d};
    while (1) {
        oled_go_to_page_and_column(0x03, 0x10);
        spi_transfer(&test);
        // oled_go_to_page_and_column(0x06, 0x05);
        // spi_transfer(&test_data2);
        // spi_transfer();
        _delay_ms(1000);
    }
    return 0;
}
