/**
 * @file ui.c
 * @author Marius Bär
 * @brief User interface implementation
 * @date 2025-09-25
 */

#include "ui.h"

#include <stddef.h>

#define ELEMENT_STACK_SIZE 4
#define EVENT_QUEUE_SIZE   5

ui_t ui;
static ui_element_t* _ui_element_stack[ELEMENT_STACK_SIZE];
static ui_event_t _ui_event_buffer[EVENT_QUEUE_SIZE];

void ui_init(void) {
    ui_ctor(&ui, _ui_element_stack, ELEMENT_STACK_SIZE, _ui_event_buffer, EVENT_QUEUE_SIZE);
}
