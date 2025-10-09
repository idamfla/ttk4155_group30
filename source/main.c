// clang-format off
// Has to be defined before the inclusion of delay.h
#include "constants.h"
// clang-format on

#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>

#include "io_board/io_board.h"
#include "max156/max156.h"
#include "oled/oled.h"
#include "spi/spi.h"
#include "ui/ui.h"
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

void on_touch_pad_data(io_touch_pad_t* touch_pad) {
    printf("Touch Pad - X: %d, Y: %d, Signal Strength: %d\r\n", touch_pad->x, touch_pad->y,
           touch_pad->signal_strength);
}

void on_button_data(io_buttons_t* buttons) {
    printf("Buttons - Left: %d, Right: %d, Nav: %d\r\n", buttons->left, buttons->right,
           buttons->nav);
}

int main(void) {
    printf_init(USART0, UBRR0);
    xmem_init();
    sei();

    spi_master_init();

    oled_init();
    ui_init();

    while (1) {
        // ui_event_push(&ui, ui_event_draw);
        //  io_get_touch_pad(on_touch_pad_data);
        //  io_get_buttons(on_button_data);
        ui_dispatch(&ui);
        _delay_ms(100);
    }
    return 0;
}
