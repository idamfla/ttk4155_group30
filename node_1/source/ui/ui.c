/**
 * @file ui.c
 * @author Marius Bär
 * @brief User interface implementation
 * @date 2025-09-25
 */

#include "ui.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "../io_board/io_board.h"
#include "elements/ui_menu_dynamic.h"
#include "elements/ui_menu_static.h"
#include "ui_constants.h"

#define ELEMENT_STACK_SIZE 4
#define EVENT_QUEUE_SIZE   5

ui_t ui;

static uint8_t _graphics_buffer[UI_BUFFER_SIZE];
static ui_element_t* _ui_element_stack[ELEMENT_STACK_SIZE];
static ui_event_t _ui_event_buffer[EVENT_QUEUE_SIZE];
static ui_menu_static_t _main_menu;
static ui_menu_static_t _sub_menu;
static ui_menu_dynamic_t _dynamic_menu;
static ui_menu_dynamic_t _game_menu;

static void main_menu_init(void);
static ui_event_status_t main_menu_on_event(ui_menu_static_t* const me, const ui_event_t event);
static ui_event_status_t sub_menu_on_event(ui_menu_static_t* const me, const ui_event_t event);
static ui_event_status_t dynamic_menu_on_event(ui_menu_dynamic_t* const me, const ui_event_t event);
static ui_event_status_t game_menu_on_event(ui_menu_dynamic_t* const me, const ui_event_t event);

volatile ui_data_t ui_data;

static void main_menu_init(void) {
    static const char* menu_items[] = {
        "Highscores:",  "IMF: 999+1",          "TAB: 999", "MAB: 42", "Game menu", "Submenu",
        "Dynamic Menu", "Init Pos", "9",   "10"};
    ui_menu_static_ctor(&_main_menu, main_menu_on_event, menu_items,
                        sizeof(menu_items) / sizeof(menu_items[0]));
}

static void sub_menu_init(void) {
    static const char* menu_items[] = {"1", "2"};
    ui_menu_static_ctor(&_sub_menu, sub_menu_on_event, menu_items,
                        sizeof(menu_items) / sizeof(menu_items[0]));
}

static void render_dynamic_menu_item(uint8_t item_index, char* buffer, uint8_t buffer_size) {
    (void)item_index;   // unused parameter
    (void)buffer_size;  // unused parameter
    if (ui_data.led2_state) {
        strcpy(buffer, "LED 2: ON");
    } else {
        strcpy(buffer, "LED 2: OFF");
    }
}

static void render_game_menu_item(uint8_t item_index, char* buffer, uint8_t buffer_size) {
    switch (item_index) {
        case 0U:
            strcpy(buffer, "(Re)Start Game");
            break;
        case 1U:
            snprintf(buffer, buffer_size, "Score: %u", ui_data.game_score);
            break;
        case 2U:
            snprintf(buffer, buffer_size, "State: %u", ui_data.game_state);
            break;
        default:
            break;
    }
}

static void dynamic_menu_init(void) {
    ui_menu_dynamic_ctor(&_dynamic_menu, dynamic_menu_on_event, 1, render_dynamic_menu_item);
}

static void game_menu_init(void) {
    ui_menu_dynamic_ctor(&_game_menu, game_menu_on_event, 3, render_game_menu_item);
}

extern volatile bool _led_state;

static ui_event_status_t main_menu_on_event(ui_menu_static_t* const me, const ui_event_t event) {
    ui_event_status_t status;
    (void)me; /* unused parameter */
    switch (event) {
        // Intentionally fall through
        case ui_event_button_right:
        case ui_event_button_select:
            switch (me->current_item) {
                case 4U:
                    ui_element_push(&ui, (ui_element_t*)&_game_menu);
                    break;
                case 5U:
                    ui_element_push(&ui, (ui_element_t*)&_sub_menu);
                    break;
                case 6U:
                    ui_element_push(&ui, (ui_element_t*)&_dynamic_menu);
                    break;
                case 7U:
                    ui_data.position_init_request = true;
                    break;
                default:
                    break;
            }
            status = ui_event_status_handled;
            break;
        default:
            status = ui_event_status_ignored;
            break;
    }
    return status;
}

static ui_event_status_t sub_menu_on_event(ui_menu_static_t* const me, const ui_event_t event) {
    ui_event_status_t status;
    (void)me; /* unused parameter */
    switch (event) {
        default:
            status = ui_event_status_ignored;
            break;
    }
    return status;
}

static ui_event_status_t dynamic_menu_on_event(ui_menu_dynamic_t* const me,
                                               const ui_event_t event) {
    ui_event_status_t status;
    switch (event) {
        case ui_event_button_select:
            switch (me->super.current_item) {
                case 0U:
                    ui_data.led2_state = !ui_data.led2_state;
                    break;
                default:
                    break;
            }
            status = ui_event_status_handled;
            break;
        default:
            status = ui_event_status_ignored;
            break;
    }
    return status;
}

static ui_event_status_t game_menu_on_event(ui_menu_dynamic_t* const me, const ui_event_t event) {
    ui_event_status_t status;
    (void)me; /* unused parameter */
    switch (event) {
        case ui_event_button_select:
            switch (me->super.current_item) {
                case 0U:
                    ui_data.start_game_request = true;
                    break;
                default:
                    break;
            }
            status = ui_event_status_handled;
            break;
        default:
            status = ui_event_status_ignored;
            break;
    }
    return status;
}

void ui_init(void) {
    ui_data.game_score = 0;
    ui_data.game_state = 0;
    ui_data.start_game_request = false;

    ui_ctor(&ui, _ui_element_stack, ELEMENT_STACK_SIZE, _ui_event_buffer, EVENT_QUEUE_SIZE,
            _graphics_buffer);
    main_menu_init();
    sub_menu_init();
    dynamic_menu_init();
    game_menu_init();

    ui_element_push(&ui, (ui_element_t*)&_main_menu);
}
