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

extern volatile bool start_game_request;  // Flag to indicate start game request

#endif /* UI_H */
