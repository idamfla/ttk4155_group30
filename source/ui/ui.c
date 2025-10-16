/**
 * @file ui.c
 * @author Marius Bär
 * @brief User interface implementation
 * @date 2025-09-25
 */

#include "ui.h"

#include <stddef.h>

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

static void main_menu_init(void);
static ui_event_status_t main_menu_on_event(ui_menu_static_t* const me, const ui_event_t event);
static ui_event_status_t sub_menu_on_event(ui_menu_static_t* const me, const ui_event_t event);

static void main_menu_init(void) {
    static const char* menu_items[] = {
        "Item 1", "Item 2", "Item 3", "Submenu", "Item 5",
        "Item 6", "Item 7", "Item 8", "Item 9",  "Item 10",
    };
    ui_menu_static_ctor(&_main_menu, main_menu_on_event, menu_items,
                        sizeof(menu_items) / sizeof(menu_items[0]));
}

static void sub_menu_init(void) {
    static const char* menu_items[] = {"Sub 1", "Sub 2", "Sub 3"};
    ui_menu_static_ctor(&_sub_menu, sub_menu_on_event, menu_items,
                        sizeof(menu_items) / sizeof(menu_items[0]));
}

static ui_event_status_t main_menu_on_event(ui_menu_static_t* const me, const ui_event_t event) {
    ui_event_status_t status;
    (void)me; /* unused parameter */
    switch (event) {
        // case ui_event_button_select:
        //     ui_element_push(&ui, (ui_element_t*)&_sub_menu);
        //     status = ui_event_status_handled;
        //     break;
        case ui_event_button_select:
            if (me->current_item == 3U) {
                ui_element_push(&ui, (ui_element_t*)&_sub_menu);
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

void ui_init(void) {
    ui_ctor(&ui, _ui_element_stack, ELEMENT_STACK_SIZE, _ui_event_buffer, EVENT_QUEUE_SIZE,
            _graphics_buffer);
    main_menu_init();
    sub_menu_init();

    ui_element_push(&ui, (ui_element_t*)&_main_menu);
}
