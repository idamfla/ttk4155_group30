/**
 * @file ui_element.h
 * @author Marius Bär
 * @brief Header file for ui_element.c
 * @date 2025-10-02
 */

#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H

#include "../ui_event.h"

typedef struct ui_element_vtable ui_element_vtable_t;
typedef struct ui_element ui_element_t;
typedef ui_event_status_t (*ui_element_on_event_t)(ui_element_t* const me, const ui_event_t event);

struct ui_element {
    const ui_element_vtable_t* vptr;
    ui_element_t* parent;
    ui_element_on_event_t on_event;
};

struct ui_element_vtable {
    void (*draw)(const ui_element_t* const me);
};

void ui_element_ctor(ui_element_t* const me, ui_element_on_event_t on_event);

/* Virtual calls (late binding) */

static inline void ui_element_draw_vcall(ui_element_t const* const me) {
    me->vptr->draw(me);
}

#endif /* UI_ELEMENT_H */
