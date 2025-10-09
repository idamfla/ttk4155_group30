/**
 * @file ui_element.c
 * @author Marius Bär
 * @brief Base class for all UI elements
 * @date 2025-10-02
 */

#include "ui_element.h"

#include <avr/pgmspace.h>
#include <stddef.h>
#include <string.h>

#include "../fonts.h"
#include "../ui_constants.h"
#include "../ui_engine.h"

static ui_event_status_t ui_element_default_on_event(ui_element_t* const me,
                                                     const ui_event_t event);
static void ui_element_draw(ui_element_t const* const me, uint8_t* const buffer, uint8_t line);

ui_element_t _ui_element_default = {
    .on_event = ui_element_default_on_event,
    .parent = NULL,
};

/**
 * @attention `on_event` must not be NULL!
 */
void ui_element_ctor(ui_element_t* const me, ui_element_on_event_t on_event) {
    static const ui_element_vtable_t vtable = {.draw = ui_element_draw};
    me->vptr = &vtable;
    me->parent = &_ui_element_default;
    me->on_event = on_event;
}

static void ui_element_draw(ui_element_t const* const me, uint8_t* const buffer, uint8_t line) {
    (void)line; /* unused parameter */
    (void)me;   /* unused parameter */
    static const char msg[] = "Draw not implemented!";
    uint8_t i = 0;
    char c = msg[i];

    memset(buffer, 0x00, UI_BUFFER_SIZE);
    while (c != '\0') {
        uint8_t idx = (uint8_t)c - FONTS_PRINTABLE_CHAR_START;
        memcpy_P(&buffer[i * 5U], &font5[idx][0], 5U);
        c = msg[++i];
    }
}

ui_event_status_t ui_element_default_on_event(ui_element_t* const me, const ui_event_t event) {
    (void)me; /* unused parameter */
    ui_event_status_t status;
    switch (event) {
        case ui_event_button_left:
            status = UI_EXIT_ELEMENT();
            break;
        default:
            status = ui_event_status_handled;
            break;
    }
    return status;
}
