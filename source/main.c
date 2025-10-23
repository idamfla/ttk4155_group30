// clang-format off
// Has to be defined before the inclusion of delay.h
#include "constants.h"
// clang-format on

#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>

#include "can/mcp2515.h"
#include "io_board/io_board.h"
#include "max156/max156.h"
#include "oled/oled.h"
#include "spi/spi.h"
#include "ui/ui.h"
#include "usart/printf.h"
#include "xmem/xmem.h"

#define BAUD        38400  // Baud rate
#define UBRR0       (F_CPU / 16 / BAUD - 1)
#define UPDATE_RATE 20U  // Hz
#include "timer/timer.h"

uint8_t test_data[] = {5};
uint8_t test_data2[] = {10};

static volatile bool _transmit_done = true;

void _spi_transfer_cmplt(void* param) {
    (void)param;  // unused
    _transmit_done = true;
}

const spi_transfer_t test = {
    .tx_data = test_data,
    .rx_data = NULL,
    .length = sizeof(test_data),
    .slave_idx = spi_slave_can,
    .transfer_cmplt_cbk = _spi_transfer_cmplt,
    .transfer_start_cbk = NULL,
};

void on_touch_pad_data(io_touch_pad_t* touch_pad) {
    printf("Touch Pad - X: %d, Y: %d, Signal Strength: %d\r\n", touch_pad->x, touch_pad->y,
           touch_pad->signal_strength);
}

void on_button_data(io_buttons_t* buttons) {
    static io_buttons_t prev_buttons = {0};
    if (buttons->nav_button && !prev_buttons.nav_button) {
        ui_event_push(&ui, ui_event_button_select);
    }
    if (buttons->nav_up && !prev_buttons.nav_up) {
        ui_event_push(&ui, ui_event_button_up);
    }
    if (buttons->nav_down && !prev_buttons.nav_down) {
        ui_event_push(&ui, ui_event_button_down);
    }
    if (buttons->nav_left && !prev_buttons.nav_left) {
        ui_event_push(&ui, ui_event_button_left);
    }
    if (buttons->nav_right && !prev_buttons.nav_right) {
        ui_event_push(&ui, ui_event_button_right);
    }
    // printf("Buttons - Left: %d, Right: %d, Nav: %d\r\n", buttons->left, buttons->right,
    //        buttons->nav);
    prev_buttons = *buttons;
}

int main(void) {
    printf_init(USART0, UBRR0);
    xmem_init();
    sei();  // Enable global interrupts

    spi_master_init();

    mcp2515_init();

    oled_init();
    ui_init();

    io_set_led_on_off(&(io_led_on_off_t){.led = 0, .on = 0}, NULL);

    timer1_init(UPDATE_RATE);
    bool cool_value = true;
    while (1) {
        // ui_event_push(&ui, ui_event_draw);
        // io_get_touch_pad(on_touch_pad_data);
        ui_dispatch(&ui);
        if (cool_value) {
            mcp2515_write(6, 0x40, 1);
            _delay_us(10);
            mcp2515_request_to_send(0x81);
            cool_value = false;
        }
        _delay_ms(500);
    }
    return 0;
}

// Executed at UPDATE_RATE Hz
ISR(TIMER1_COMPA_vect) {
    io_get_buttons(on_button_data);
    ui_event_push(&ui, ui_event_draw);
}