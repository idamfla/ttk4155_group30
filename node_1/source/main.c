// clang-format off
// Has to be defined before the inclusion of delay.h
#include "constants.h"
// clang-format on

#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>

// #include "can/mcp2515.h"
#include "can/can.h"
#include "io_board/io_board.h"
#include "max156/max156.h"
#include "oled/oled.h"
#include "spi/spi.h"
#include "ui/ui.h"
#include "usart/printf.h"
#include "xmem/xmem.h"
#include "game/game.h"

#define BAUD        38400  // Baud rate
#define UBRR0       (F_CPU / 16 / BAUD - 1)
#define UPDATE_RATE 20U  // Hz
#include "timer/timer.h"
uint8_t arr[1] = {0x01};
uint8_t test_data[] = {5};
// CAN_DATA test_data2 = {.id = 0b10011101101, .data = arr, .length = 1};

max156_data_t max156_data;

static volatile bool _transmit_done = true;
volatile bool can_joystick_flag = false;
volatile bool can_rx_flag = false;
volatile bool io_get_buttons_flag = false;
volatile bool ui_event_push_flag = false;

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

game_inputs_t game_inputs;
can_message_t _can_msg = {
    .id = 0x1,
    .data = (uint8_t*)&game_inputs,
    .length = 3,
};

static volatile io_buttons_t prev_buttons = {0};

void on_button_data(io_buttons_t* buttons) {
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

void transfer_states(void) {
    max156_trigger_conversion();
    max156_read(&max156_data);
    // printf("Button: SR2=%d, SR3=%d\r\n", prev_buttons.SR2, prev_buttons.SR3);
    game_inputs.pos_joystick = max156_data.ch3;
    game_inputs.pos_slider = max156_data.ch0;
    if (prev_buttons.SR3) {
        game_inputs.cmd = game_cmd_reset;  // reset command
    } else if (ui_data.position_init_request) {
        game_inputs.cmd = game_cmd_init_pos;  // Init position command
        ui_data.position_init_request = false;
    } else if (ui_data.start_game_request) {
        game_inputs.cmd = game_cmd_start_game;  // Start game command
        ui_data.start_game_request = false;
    } else {
        game_inputs.cmd = game_cmd_none;  // No command
    }
    game_inputs.solenoid_out = prev_buttons.SR2;  // Solenoid output
    can_send(&_can_msg);
}

volatile bool _led_state = 0;
bool _prev_led_state = 0;

void can_rx_cmplt(can_message_t* can_msg) {
    game_outputs_t* game_outputs = (game_outputs_t*)can_msg->data;
    ui_data.game_state = game_outputs->game.state;
    ui_data.game_score = can_msg->data[0] << 8 | can_msg->data[1];
    // printf("Score: %u, State: %u\r\n", ui_data.game_score, ui_data.game_state);
    // printf("ID: %d, Length: %d, Data: [", can_msg->id, can_msg->length);
    // for (size_t i = 0; i < can_msg->length; i++) {
    //     printf("%d, ", can_msg->data[i]);
    // }
    // printf("]\r\n");
}

int main(void) {
    printf_init(USART0, UBRR0);
    xmem_init();
    sei();  // Enable global interrupts

    spi_master_init();

    // mcp2515_init();

    oled_init();
    ui_init();

    max156_init();

    // CAN_init(can_rx_cmplt);
    can_init(can_rx_cmplt);
    io_set_led_on_off(&(io_led_on_off_t){.led = 0, .on = false}, NULL);
    io_set_led_on_off(&(io_led_on_off_t){.led = 1, .on = false}, NULL);

    timer1_init(UPDATE_RATE);
    // printf("Starting main loop\r\n");

    // can_send(&_can_msg);

    while (1) {
        if (can_receive_pending()) {
            can_receive();
        }
        // can_state_t state = can_get_state();
        // io_get_buttons(on_button_data);
        // if (state == can_state_idle) {
        //     _delay_ms(1000);
        //     can_init();
        // }
        // printf("Can state: %d\r\n", state);
        ui_dispatch(&ui);
        if (ui_data.led2_state != _prev_led_state) {
            if (io_set_led_on_off(&(io_led_on_off_t){.led = 1, .on = ui_data.led2_state}, NULL)) {
                _prev_led_state = ui_data.led2_state;
            }
        }
    }
    return 0;
}

// Executed at UPDATE_RATE Hz
ISR(TIMER1_COMPA_vect) {
    io_get_buttons(on_button_data);
    ui_event_push(&ui, ui_event_draw);
    transfer_states();
}