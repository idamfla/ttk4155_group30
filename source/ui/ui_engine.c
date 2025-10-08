/**
 * @file ui_engine.c
 * @author Marius Bär
 * @brief UI engine
 * @date 2025-10-02
 */

#include "ui_engine.h"

#include <stddef.h>

#include "ui_constants.h"

static ui_event_t ui_event_pop(ui_t* const me);
static ui_event_status_t ui_send_event(ui_element_t* element, const ui_event_t event);

void ui_ctor(ui_t* const me, ui_element_t** stack, uint8_t stack_capacity, ui_event_t* event_buffer,
             uint8_t event_queue_capacity) {
    me->element_stack.stack = stack;
    // Stack grows downwards
    me->element_stack.stack_top = stack + stack_capacity;
    me->element_stack.stack_capacity = stack_capacity;

    me->event_queue.buffer = event_buffer;
    me->event_queue.back = event_buffer;
    me->event_queue.front = event_buffer;
    me->event_queue.max_size = event_queue_capacity;
    me->event_queue.size = 0;
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

bool ui_event_push(ui_t* const me, const ui_event_t event) {
    ui_event_queue_t* queue = &me->event_queue;
    if (queue->size >= queue->max_size) {
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
    return true;
}

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

void ui_draw_complete(ui_element_t* const me) {
    if (me->line >= UI_DISPLAY_LINES) {
        me->line = 0U;
    } else {
        ui_element_draw_vcall(me, me->line++);
    }
}

static ui_event_status_t ui_send_event(ui_element_t* element, const ui_event_t event) {
    ui_event_status_t status;
    do {
        element = element->parent;
        status = element->on_event(element, event);
        // No check for parent == NULL needed, as the root element must handle all events
    } while (status == ui_event_status_ignored);
    return status;
}

void ui_dispatch(ui_t* const me) {
    if (me->event_queue.size == 0) {
        return;
    }
    ui_event_t event = ui_event_pop(me);
    ui_element_t* active_element = (*me->element_stack.stack_top);
    if (ui_send_event(active_element, event) == ui_event_status_element_exit) {
        ui_element_pop(me);
    }
}
