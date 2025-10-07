/**
 * @file ui_element.c
 * @author Marius Bär
 * @brief Base class for all UI elements
 * @date 2025-10-02
 */

#include "ui_element.h"

static void ui_element_draw(ui_element_t const* const me);

void ui_element_ctor(ui_element_t* const me, ui_on_event_t on_event) {
    static const ui_element_vtable_t vtable = {.draw = ui_element_draw};
    me->vptr = &vtable;
    me->on_event = on_event;
}

static void ui_element_draw(ui_element_t const* const me) {
    // TODO: Implement a default draw function that indicates an error
    (void)me; /* unused parameter */
}
