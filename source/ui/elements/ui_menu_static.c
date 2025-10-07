/**
 * @file ui_menu_static.c
 * @author Marius Bär
 * @brief Menu with static content
 * @date 2025-10-02
 */

#include "ui_menu_static.h"

void ui_menu_static_ctor(ui_menu_static_t *const me, ui_on_event_t on_event,
                         const char **const item_texts, uint8_t num_items) {
    static const ui_element_vtable_t vtable = {
        .draw = (void (*)(const ui_element_t *const))ui_menu_static_draw};
    ui_element_ctor(&me->super, on_event);

    me->super.vptr = &vtable;
    me->item_texts = item_texts;
    me->num_items = num_items;
    me->scroll_pos = 0U;
    me->current_item = 0U;
}

void ui_menu_static_draw(ui_menu_static_t const *const me) {
    // TODO: this needs to be implemented
    (void)me; /* unused parameter */
}
