/**
 * @file ui_menu_static.c
 * @author Marius Bär
 * @brief Menu with static content
 * @date 2025-10-02
 */

#include "ui_menu_static.h"

#include <stddef.h>
#include <string.h>

#include "../fonts.h"
#include "../ui_constants.h"

extern ui_element_t _ui_element_default;
static ui_event_status_t ui_menu_static_default_on_event(ui_menu_static_t *const me,
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
        .draw = (void (*)(const ui_element_t *const, uint8_t *const, uint8_t))ui_menu_static_draw};

    ui_element_ctor(&me->super, (ui_element_on_event_t)on_event);

    me->super.vptr = &vtable;
    me->super.parent = (ui_element_t *)&_ui_menu_static_default;
    me->item_texts = item_texts;
    me->num_items = num_items;
    me->scroll_pos = 0U;
    me->current_item = 0U;
}

static ui_event_status_t ui_menu_static_default_on_event(ui_menu_static_t *const me,
                                                         const ui_event_t event) {
    ui_event_status_t status;
    switch (event) {
        case ui_event_element_entry:
            me->current_item = 0U;
            me->scroll_pos = 0U;
            status = ui_event_status_handled;
            break;
        case ui_event_button_down:
            if (me->current_item < me->num_items - 1U) {
                ++me->current_item;
                // If the selected item is at the edge or outside of the visible area, scroll down
                // (only if not at the last item).
                if ((uint8_t)(me->current_item - me->scroll_pos) >= UI_DISPLAY_LINES - 1U &&
                    me->current_item < me->num_items - 1U) {
                    ++me->scroll_pos;
                }
            } else {
                // Wrap around to the first item
                me->current_item = 0;
                me->scroll_pos = 0;
            }
            status = ui_event_status_handled;
            break;
        case ui_event_button_up:
            if (me->current_item != 0U) {
                --me->current_item;
                // If the selected item is at the edge of the visible area, scroll up (only if not
                // at the first item).
                if ((uint8_t)(me->current_item - me->scroll_pos) == 0U && me->current_item != 0U) {
                    --me->scroll_pos;
                }
            } else {
                // Wrap around to the last item
                me->current_item = me->num_items - 1U;
                if (me->num_items > UI_DISPLAY_LINES) {
                    me->scroll_pos = me->num_items - UI_DISPLAY_LINES;
                } else {
                    me->scroll_pos = 0U;
                }
            }
            status = ui_event_status_handled;
            break;
        default:
            status = ui_event_status_ignored;
            break;
    }
    return status;
}

void ui_menu_static_draw(ui_menu_static_t const *const me, uint8_t *const buffer, uint8_t line) {
    uint8_t item = me->scroll_pos + line;
    if (item == me->current_item) {
        FONTS_CPY_CHAR(font5, &buffer[0], '>');
    } else {
        FONTS_CPY_CHAR(font5, &buffer[0], ' ');
    }
    FONTS_CPY_CHAR(font5, &buffer[1 * 5U], ' ');
    const char *text = (item < me->num_items) ? me->item_texts[item] : "";

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
