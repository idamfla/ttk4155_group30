/**
 * @file ui_element.c
 * @author Marius Bär
 * @brief Base class for all UI elements
 * @date 2025-10-02
 */

#include "ui_element.h"

#include <stddef.h>

static ui_event_status_t ui_element_default_on_event(ui_element_t* const me,
                                                     const ui_event_t event);
static void ui_element_draw(ui_element_t const* const me);

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

static void ui_element_draw(ui_element_t const* const me) {
    // TODO: Implement a default draw function that indicates an error
    (void)me; /* unused parameter */
}

ui_event_status_t ui_element_default_on_event(ui_element_t* const me, const ui_event_t event) {
    (void)me;    /* unused parameter */
    (void)event; /* unused parameter */
    return ui_event_status_handled;
}
