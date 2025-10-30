/**
 * @file ui_engine.h
 * @author Marius Bär
 * @brief Header file for ui_engine.c
 * @date 2025-09-25
 */

#ifndef UI_ENGINE_H
#define UI_ENGINE_H

#include <stdbool.h>
#include <stdint.h>

#include "elements/ui_element.h"
#include "ui_event.h"

#define UI_EXIT_ELEMENT() ui_event_status_element_exit

typedef struct {
    volatile ui_event_t* buffer;
    volatile ui_event_t* volatile back;
    volatile ui_event_t* volatile front;
    uint8_t max_size;
    volatile uint8_t size;
} ui_event_queue_t;

typedef struct {
    ui_element_t* volatile* stack;
    ui_element_t* volatile* volatile stack_top;
    uint8_t stack_capacity;
} ui_element_stack_t;

typedef struct {
    ui_element_stack_t element_stack;
    ui_event_queue_t event_queue;
    uint8_t* graphics_buffer;
    uint8_t line;
} ui_t;

void ui_ctor(ui_t* const me, ui_element_t** stack, uint8_t stack_capacity, ui_event_t* event_buffer,
             uint8_t event_queue_capacity, uint8_t* const graphics_buffer);
bool ui_element_push(ui_t* const me, ui_element_t* const element);
void ui_element_pop(ui_t* const me);
bool ui_event_push(ui_t* const me, const ui_event_t event);
void ui_draw_complete(ui_t* const me);
void ui_dispatch(ui_t* const me);

#endif /* UI_ENGINE_H */
