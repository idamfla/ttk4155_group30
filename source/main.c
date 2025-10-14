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
#define FPS   20

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
    static io_buttons_t prev_buttons = {0};
    if (buttons->SR5 && !prev_buttons.SR5) {
        ui_event_push(&ui, ui_event_button_up);
    }
    if (buttons->SR6 && !prev_buttons.SR6) {
        ui_event_push(&ui, ui_event_button_down);
    }
    if (buttons->SR4 && !prev_buttons.SR4) {
        ui_event_push(&ui, ui_event_button_select);
    }
    if (buttons->SR1 && !prev_buttons.SR1) {
        ui_event_push(&ui, ui_event_button_left);
    }
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

    oled_init();
    ui_init();

    TCCR1B |= (1 << CS10) | (1 << CS11);  // Prescaler = 64
    TCCR1B |= (1 << WGM12);               // Set WGM12 bit for CTC mode (Mode 4)
    OCR1A = F_CPU / 64 / FPS - 1;         // Set compare value for desired frequency
    TIMSK |= (1 << OCIE1A);               // Enable Timer1 Compare A interrupt

    while (1) {
        // ui_event_push(&ui, ui_event_draw);
        // io_get_touch_pad(on_touch_pad_data);
        ui_dispatch(&ui);
        // _delay_ms(500);
    }
    return 0;
}

// Execute every 1/FPS seconds
ISR(TIMER1_COMPA_vect) {
    io_get_buttons(on_button_data);
    ui_event_push(&ui, ui_event_draw);
}