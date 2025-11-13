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

static void main_menu_init(void);
static ui_event_status_t main_menu_on_event(ui_menu_static_t* const me, const ui_event_t event);
static ui_event_status_t sub_menu_on_event(ui_menu_static_t* const me, const ui_event_t event);
static ui_event_status_t dynamic_menu_on_event(ui_menu_dynamic_t* const me, const ui_event_t event);

bool led2_state = false;

static void main_menu_init(void) {
    static const char* menu_items[] = {
        "LED 1: on", "LED 1: off", "Item 3", "Submenu", "Dynamic Menu",
        "Item 6",    "Item 7",     "Item 8", "Item 9",  "Item 10",
    };
    ui_menu_static_ctor(&_main_menu, main_menu_on_event, menu_items,
                        sizeof(menu_items) / sizeof(menu_items[0]));
}

static void sub_menu_init(void) {
    static const char* menu_items[] = {"Sub 1", "Sub 2", "Sub 3"};
    ui_menu_static_ctor(&_sub_menu, sub_menu_on_event, menu_items,
                        sizeof(menu_items) / sizeof(menu_items[0]));
}

static void render_dynamic_item(uint8_t item_index, char* buffer, uint8_t buffer_size) {
    static uint16_t counter = 0U;
    // Simple rendering: just write "Item X" where X is the item index
    switch (item_index) {
        case 0U:
            if (led2_state) {
                strcpy(buffer, "LED 2: ON");
            } else {
                strcpy(buffer, "LED 2: OFF");
            }
            break;
        case 1U:
            snprintf(buffer, buffer_size, "Counter: %u", counter++);
            break;
        default:
            break;
            if (item_index == 0U) {
                ++counter;
            }
    }
}

static void dynamic_menu_init(void) {
    ui_menu_dynamic_ctor(&_dynamic_menu, dynamic_menu_on_event, 2, render_dynamic_item);
}

static ui_event_status_t main_menu_on_event(ui_menu_static_t* const me, const ui_event_t event) {
    ui_event_status_t status;
    (void)me; /* unused parameter */
    switch (event) {
        // Intentionally fall through
        case ui_event_button_right:
        case ui_event_button_select:
            switch (me->current_item) {
                case 0U:
                    io_set_led_on_off(&(io_led_on_off_t){.led = 0, .on = 1}, NULL);
                    break;
                case 1U:
                    io_set_led_on_off(&(io_led_on_off_t){.led = 0, .on = 0}, NULL);
                    break;
                case 3U:
                    ui_element_push(&ui, (ui_element_t*)&_sub_menu);
                    break;
                case 4U:
                    ui_element_push(&ui, (ui_element_t*)&_dynamic_menu);
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
        // case ui_event_button_select:
        //     ui_element_push(&ui, (ui_element_t*)&_sub_menu);
        //     status = ui_event_status_handled;
        //     break;
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
                    led2_state = !led2_state;
                    io_set_led_on_off(&(io_led_on_off_t){.led = 1, .on = led2_state}, NULL);
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
    ui_ctor(&ui, _ui_element_stack, ELEMENT_STACK_SIZE, _ui_event_buffer, EVENT_QUEUE_SIZE,
            _graphics_buffer);
    main_menu_init();
    sub_menu_init();
    dynamic_menu_init();

    ui_element_push(&ui, (ui_element_t*)&_main_menu);
}
