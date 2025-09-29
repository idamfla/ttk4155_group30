/**
 * @file ui_engine.h
 * @author Marius Bär
 * @brief Header file for ui_engine.c
 * @date 2025-09-25
 */

#ifndef UI_ENGINE_H
#define UI_ENGINE_H

#include <stdint.h>

typedef struct {
    const char* text;
    void (*action)(void);
} ui_menu_item_t;

typedef struct {
    ui_menu_item_t* items;
    uint8_t num_items;
} ui_menu_t;

#endif /* UI_ENGINE_H */
