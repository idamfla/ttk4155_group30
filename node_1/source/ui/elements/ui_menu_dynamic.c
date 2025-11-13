/**
 * @file ui_menu_dynamic.c
 * @author Marius Bär
 * @brief Menu with dynamic content
 * @date 2025-10-02
 */

#include "ui_menu_dynamic.h"

#include <stddef.h>
#include <string.h>

#include "../fonts.h"
#include "../ui_constants.h"

extern ui_element_t _ui_menu_static_default;
static ui_event_status_t ui_menu_dynamic_default_on_event(ui_menu_dynamic_t* const me,
                                                          const ui_event_t event);

ui_menu_dynamic_t _ui_menu_dynamic_default = {
    .super.super =
        {
            .on_event = (ui_element_on_event_t)ui_menu_dynamic_default_on_event,
            .parent = &_ui_menu_static_default,
        },
};

/**
 * @attention `on_event` must not be NULL!
 */
void ui_menu_dynamic_ctor(ui_menu_dynamic_t* const me, ui_menu_dynamic_on_event_t on_event,
                          uint8_t num_items,
                          void (*render_item_cbk)(uint8_t item_index, char* buffer,
                                                  uint8_t buffer_size)) {
    static const ui_element_vtable_t vtable = {
        .draw = (void (*)(const ui_element_t* const, uint8_t* const, uint8_t))ui_menu_dynamic_draw};

    ui_menu_static_ctor(&me->super, (ui_menu_static_on_event_t)on_event, NULL, num_items);

    me->super.super.vptr = &vtable;
    me->super.super.parent = (ui_element_t*)&_ui_menu_dynamic_default;
    me->render_item_cbk = render_item_cbk;
}

static ui_event_status_t ui_menu_dynamic_default_on_event(ui_menu_dynamic_t* const me,
                                                          const ui_event_t event) {
    (void)me;
    ui_event_status_t status;
    switch (event) {
        default:
            status = ui_event_status_ignored;
            break;
    }
    return status;
}

void ui_menu_dynamic_draw(ui_menu_dynamic_t const* const me, uint8_t* const buffer, uint8_t line) {
    char text[25];
    uint8_t item = me->super.scroll_pos + line;
    if (item == me->super.current_item) {
        FONTS_CPY_CHAR(font5, &buffer[0], '>');
    } else {
        FONTS_CPY_CHAR(font5, &buffer[0], ' ');
    }
    FONTS_CPY_CHAR(font5, &buffer[1 * 5U], ' ');
    if (item < me->super.num_items) {
        me->render_item_cbk(item, text, sizeof(text));
    } else {
        text[0] = '\0';
    }

    uint8_t i = 0;
    uint8_t j = (i + 2) * 5U;
    char c = text[i];
    while (c != '\0') {
        FONTS_CPY_CHAR(font5, &buffer[j], c);
        j += 5U;
        c = text[++i];
    }
    memset(&buffer[j], 0, UI_BUFFER_SIZE - j);
}
