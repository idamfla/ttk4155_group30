/**
 * @file ui_menu_static.h
 * @author Marius Bär
 * @brief Header file for ui_menu_static.c
 * @date 2025-10-02
 */

#ifndef UI_MENU_STATIC_H
#define UI_MENU_STATIC_H

#include <stdint.h>

#include "ui_element.h"

typedef struct {
    ui_element_t super;
    const char** item_texts;
    uint8_t num_items;
    uint8_t current_item;
    uint8_t scroll_pos;
} ui_menu_static_t;

typedef ui_event_status_t (*ui_menu_static_on_event_t)(ui_menu_static_t* const me,
                                                       const ui_event_t event);

void ui_menu_static_ctor(ui_menu_static_t* const me, ui_menu_static_on_event_t on_event,
                         const char** const item_texts, uint8_t num_items);
void ui_menu_static_draw(ui_menu_static_t const* const me);

#endif /* UI_MENU_STATIC_H */
