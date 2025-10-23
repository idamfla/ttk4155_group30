/**
 * @file ui_engine.c
 * @author Marius Bär
 * @brief UI engine
 * @date 2025-10-02
 */

#include "ui_engine.h"

#include <avr/interrupt.h>
#include <stddef.h>

#include "../oled/oled.h"
#include "ui_constants.h"

static ui_event_t ui_event_pop(ui_t* const me);
static ui_event_status_t ui_send_event(ui_element_t* element, const ui_event_t event);

void ui_ctor(ui_t* const me, ui_element_t** stack, uint8_t stack_capacity, ui_event_t* event_buffer,
             uint8_t event_queue_capacity, uint8_t* const graphics_buffer) {
    me->element_stack.stack = stack;
    // Stack grows downwards
    me->element_stack.stack_top = stack + stack_capacity;
    me->element_stack.stack_capacity = stack_capacity;

    me->event_queue.buffer = event_buffer;
    me->event_queue.back = event_buffer;
    me->event_queue.front = event_buffer;
    me->event_queue.max_size = event_queue_capacity;
    me->event_queue.size = 0;

    me->graphics_buffer = graphics_buffer;
    me->line = 0U;
}

bool ui_element_push(ui_t* const me, ui_element_t* const element) {
    // Stack full
    if (me->element_stack.stack_top == me->element_stack.stack) {
        return false;
    }
    *(--me->element_stack.stack_top) = element;
    ui_send_event(element, ui_event_element_entry);
    return true;
}

void ui_element_pop(ui_t* const me) {
    // Stack empty
    if (me->element_stack.stack_top == me->element_stack.stack + me->element_stack.stack_capacity) {
        return;
    }
    ui_send_event(*me->element_stack.stack_top, ui_event_element_exit);
    ++me->element_stack.stack_top;
    // Re-enter the new top element (Not implemented yet)
    // ui_send_event(*me->element_stack.stack_top, ui_event_element_reentry);
}

static inline uint8_t ui_element_stack_size(const ui_t* const me) {
    return (uint8_t)(me->element_stack.stack + me->element_stack.stack_capacity -
                     me->element_stack.stack_top);
}

bool ui_event_push(ui_t* const me, const ui_event_t event) {
    ui_event_queue_t* queue = &me->event_queue;
    cli();  // Disable interrupts
    if (queue->size >= queue->max_size) {
        sei();  // Enable interrupts
        return false;
    }
    ++queue->size;
    // Fill the queue backwards
    *queue->back = event;
    if (queue->back > queue->buffer) {
        --queue->back;
    } else {
        queue->back = queue->buffer + queue->max_size - 1;
    }
    sei();  // Enable interrupts
    return true;
}

/**
 * @brief Pop an event from the event queue
 * @param me Pointer to the ui instance
 * @return The event at the front of the queue
 * @attention This function does not check if the queue is empty. The caller must ensure that the
 * queue is not empty.
 * @attention This function must be called with interrupts disabled.
 */
static ui_event_t ui_event_pop(ui_t* const me) {
    ui_event_queue_t* queue = &me->event_queue;
    ui_event_t front = *(queue->front);
    if (queue->front > queue->buffer) {
        --queue->front;
    } else {
        queue->front = queue->buffer + queue->max_size - 1;
    }
    --queue->size;
    return front;
}

void ui_draw_complete(ui_t* const me) {
    ui_element_t* active_element = (*me->element_stack.stack_top);
    if (me->line >= UI_DISPLAY_LINES) {
        me->line = 0U;
    } else {
        ui_element_draw_vcall(active_element, me->graphics_buffer, me->line++);
        oled_write_to_display(me->graphics_buffer, UI_BUFFER_SIZE,
                              (void (*)(void*))ui_draw_complete, me);
    }
}

static ui_event_status_t ui_send_event(ui_element_t* element, const ui_event_t event) {
    ui_event_status_t status;
    ui_element_t* parent_element = element;
    do {
        status = parent_element->on_event(element, event);
        parent_element = parent_element->parent;
        // No check for parent == NULL needed, as the root element must handle all events
    } while (status == ui_event_status_ignored);
    return status;
}

void ui_dispatch(ui_t* const me) {
    cli();  // Disable interrupts
    if (me->event_queue.size == 0) {
        sei();  // Enable interrupts
        return;
    }
    ui_event_t event = ui_event_pop(me);
    sei();  // Enable interrupts

    ui_element_t* active_element = (*me->element_stack.stack_top);
    if (event == ui_event_draw) {
        if (me->line != 0) {
            return;
        }
        ui_element_draw_vcall(active_element, me->graphics_buffer, me->line++);
        oled_write_to_display(me->graphics_buffer, UI_BUFFER_SIZE,
                              (void (*)(void*))ui_draw_complete, me);
    } else if (ui_send_event(active_element, event) == ui_event_status_element_exit &&
               ui_element_stack_size(me) > 1U) {
        ui_element_pop(me);
    }
}
