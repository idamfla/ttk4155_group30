/**
 * @file ui_menu_dynamic.h
 * @author Marius Bär
 * @brief Header file for ui_menu_dynamic.c
 * @date 2025-11-13
 */

#ifndef UI_MENU_DYNAMIC_H
#define UI_MENU_DYNAMIC_H

#include <stdint.h>

#include "ui_menu_static.h"

typedef struct {
    ui_menu_static_t super;
    void(*render_item_cbk)(uint8_t item_index, char* buffer, uint8_t buffer_size);
} ui_menu_dynamic_t;

typedef ui_event_status_t (*ui_menu_dynamic_on_event_t)(ui_menu_dynamic_t* const me,
                                                       const ui_event_t event);

void ui_menu_dynamic_ctor(ui_menu_dynamic_t* const me, ui_menu_dynamic_on_event_t on_event,
                          uint8_t num_items,
                          void (*render_item_cbk)(uint8_t item_index, char* buffer,
                                                  uint8_t buffer_size));
void ui_menu_dynamic_draw(ui_menu_dynamic_t const* const me, uint8_t* const buffer, uint8_t line);

#endif /* UI_MENU_DYNAMIC_H */
