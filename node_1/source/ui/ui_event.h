/**
 * @file ui_event.h
 * @author Marius Bär
 * @brief UI event definitions
 * @date 2025-10-07
 */

#ifndef UI_EVENT_H
#define UI_EVENT_H

typedef enum {
    ui_event_element_entry,
    ui_event_element_exit,
    ui_event_draw,
    ui_event_button_up,
    ui_event_button_down,
    ui_event_button_left,
    ui_event_button_right,
    ui_event_button_select,
} ui_event_t;

typedef enum {
    ui_event_status_ignored,
    ui_event_status_handled,
    ui_event_status_element_exit,
} ui_event_status_t;

#endif /* UI_EVENT_H */
