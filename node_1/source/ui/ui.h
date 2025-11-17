/**
 * @file ui.h
 * @author Marius Bär
 * @brief Header file for ui.c
 * @date 2025-09-25
 */

#ifndef UI_H
#define UI_H

#include "ui_engine.h"

extern ui_t ui;

void ui_init(void);

typedef struct {
    bool start_game_request;
    bool position_init_request;
    uint16_t game_score;
    uint8_t game_state;
    bool led2_state;
} ui_data_t;

extern volatile ui_data_t ui_data;

#endif /* UI_H */
