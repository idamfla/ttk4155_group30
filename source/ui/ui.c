/**
 * @file ui.c
 * @author Marius Bär
 * @brief User interface implementation
 * @date 2025-09-25
 */

#include "ui.h"

#include <stddef.h>

#include "ui_engine.h"

static ui_menu_item_t main_menu_items[] = {
    {"Item 1", NULL},
    {"Item 2", NULL},
    {"Item 3", NULL},
};

static ui_menu_t main_menu = {
    .items = main_menu_items,
    .num_items = sizeof(main_menu_items) / sizeof(main_menu_items[0]),
};

void ui_render(void) {}