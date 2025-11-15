/**
 * @file game.c
 * @author Marius Bär
 * @brief Game logic for the ping-pong game
 * @date 2025-11-15
 */

#include "game.h"

#include "ir/ir.h"

#define GAME_IR_THRESHOLD  1000
#define GAME_SCORE_CLK_DIV 20

// pos joystick: 66 - 246
// pos slider: 0 - 255

void game_init(volatile game_t* game) {
    game->score = 0;
    game->state = game_pos_invalid;
}

void game_update(volatile game_t* game, game_inputs_t* inputs) {
    static uint32_t score_sub_counter;
    switch (game->state) {
        case game_pos_invalid:
            // Turn off motor
            // If init pos command received, change state to game_init_pos
            break;

        case game_idle:
            // Disable motor
            break;

        case game_init_pos:
            // Slowly move to the negative endstop
            break;

        case game_wait_for_endstop:
            // If endstop reached:
            // Turn off motor
            // Reset position to 0
            // Set state to game_idle
            break;

        case game_start_game:
            score_sub_counter = 0;
            // Set low speed
            // Set position to edge, set position of servo and retract solenoid
            // if position is reached, change state to waiting for start
            // if blocked, turn off motor and change state to error
            break;

        case game_waiting_for_start:
            if (inputs->solenoid_out) {
                // Extend solenoid
                game->state = game_active;
            }
            break;

        case game_active:
            if (++score_sub_counter >= GAME_SCORE_CLK_DIV) {
                score_sub_counter = 0;
                ++game->score;
            }
            if (ir_read() < GAME_IR_THRESHOLD) {
                // Turn motor off
                game->state = game_lost;
            }
            break;

        case game_lost:
            if (inputs->reset) {
                game->score = 0;
                game->state = game_start_game;
            }
            break;

        case game_error:
            // Turn motor off
            if (inputs->reset) {
                game->state = game_idle;
            }
            break;

        default:
            game->state = game_error;
            break;
    }
}
