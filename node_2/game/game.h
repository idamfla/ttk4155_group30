/**
 * @file game.h
 * @author Marius Bär
 * @brief Header file for game.c
 * @date 2025-11-15
 */

#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef enum {
    game_pos_invalid,
    game_idle,
    game_init_pos,
    game_wait_for_endstop,
    game_start_game,
    game_waiting_for_start,
    game_active,
    game_lost,
    game_error
} game_state_t;

typedef struct __attribute__((packed)) {
    uint16_t score;
    uint8_t state;
} game_t;
_Static_assert(sizeof(game_t) == 3, "Size of game_t must be 3 bytes");

typedef struct __attribute__((packed)) {
    uint8_t pos_joystick;
    uint8_t pos_slider;
    struct {
        uint8_t start : 1;         // Initiate new round
        uint8_t reset : 1;         // Reset game from error state
        uint8_t solenoid_out : 1;  // Extend solenoid
        uint8_t : 5;
    };
} game_inputs_t;
_Static_assert(sizeof(game_inputs_t) == 3, "Size of game_inputs_t must be 3 bytes");

typedef struct __attribute__((packed)) {
    game_t game;
} game_outputs_t;
_Static_assert(sizeof(game_outputs_t) == 3, "Size of game_cmd_t must be 3 bytes");

void game_init(volatile game_t* game);
void game_update(volatile game_t* game, game_inputs_t* inputs);
