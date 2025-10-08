/**
 * @file ui_menu_static.c
 * @author Marius Bär
 * @brief Menu with static content
 * @date 2025-10-02
 */

#include "ui_menu_static.h"

#include <stddef.h>

extern ui_element_t _ui_element_default;
static ui_event_status_t ui_menu_static_default_on_event(ui_menu_static_on_event_t *const me,
                                                         const ui_event_t event);

ui_menu_static_t _ui_menu_static_default = {
    .super =
        {
            .on_event = (ui_element_on_event_t)ui_menu_static_default_on_event,
            .parent = &_ui_element_default,
        },
};

/**
 * @attention `on_event` must not be NULL!
 */
void ui_menu_static_ctor(ui_menu_static_t *const me, ui_menu_static_on_event_t on_event,
                         const char **const item_texts, uint8_t num_items) {
    static const ui_element_vtable_t vtable = {
        .draw = (void (*)(const ui_element_t *const))ui_menu_static_draw};
    ui_element_ctor(&me->super, (ui_element_on_event_t)on_event);

    me->super.vptr = &vtable;
    me->super.parent = (ui_element_t *)&_ui_menu_static_default;
    me->item_texts = item_texts;
    me->num_items = num_items;
    me->scroll_pos = 0U;
    me->current_item = 0U;
}

static ui_event_status_t ui_menu_static_default_on_event(ui_menu_static_on_event_t *const me,
                                                         const ui_event_t event) {
    (void)me;
    (void)event;
    return ui_event_status_ignored;
}

void ui_menu_static_draw(ui_menu_static_t const *const me) {
    // TODO: this needs to be implemented
    (void)me; /* unused parameter */
}
