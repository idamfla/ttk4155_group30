/**
 * @file ui.c
 * @author Marius Bär
 * @brief User interface implementation
 * @date 2025-09-25
 */

#include "ui.h"

#include <stddef.h>

#include "elements/ui_menu_static.h"

#define ELEMENT_STACK_SIZE 4
#define EVENT_QUEUE_SIZE   5

ui_t ui;

static ui_element_t* _ui_element_stack[ELEMENT_STACK_SIZE];
static ui_event_t _ui_event_buffer[EVENT_QUEUE_SIZE];
static ui_menu_static_t _main_menu;

static void main_menu_init(void);
static ui_event_status_t main_menu_on_event(ui_menu_static_t* const me, const ui_event_t event);

static void main_menu_init(void) {
    static const char* menu_items[] = {
        "Item 1",
        "Item 2",
        "Item 3",
    };
    ui_menu_static_ctor(&_main_menu, main_menu_on_event, menu_items,
                        sizeof(menu_items) / sizeof(menu_items[0]));
}

static ui_event_status_t main_menu_on_event(ui_menu_static_t* const me, const ui_event_t event) {
    ui_event_status_t status;
    (void)me; /* unused parameter */
    switch (event) {
        default:
            status = ui_event_status_ignored;
            break;
    }
    return status;
}

void ui_init(void) {
    ui_ctor(&ui, _ui_element_stack, ELEMENT_STACK_SIZE, _ui_event_buffer, EVENT_QUEUE_SIZE);
    main_menu_init();

    ui_element_push(&ui, (ui_element_t*)&_main_menu);
}
