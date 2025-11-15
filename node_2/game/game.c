/**
 * @file game.c
 * @author Marius Bär
 * @brief Game logic for the ping-pong game
 * @date 2025-11-15
 */

#include "game.h"

#include "constants.h"
#include "ir/ir.h"
#include "pi_controller/motor.h"
#include "pwm/pwm.h"
#include "quad_encoder/quad_encoder.h"
#include "solenoid/solenoid.h"

#define GAME_IR_THRESHOLD  1000
#define GAME_SCORE_CLK_DIV 20

#define ABS(x) ((x) < 0 ? -(x) : (x))

// pos joystick: 66 - 246
// pos slider: 0 - 255

void game_init(volatile game_t* game) {
    game->score = 0;
    game->state = game_pos_invalid;
    motor_init(MOTOR_SPEED_SLOW, MOTOR_CURRENT_LOW);
}

void game_update(volatile game_t* game, game_inputs_t* inputs) {
    static uint32_t score_sub_counter;
    static uint32_t motor_stopped_counter;
    switch (game->state) {
        case game_pos_invalid:
            motor_init(MOTOR_SPEED_SLOW, MOTOR_CURRENT_LOW);
            pwm_set_dc_servo(CDTY1_MIDDLE);
            if (inputs->cmd == game_cmd_init_pos) {
                motor_stopped_counter = 0;
                motor_ctrl_speed(-MOTOR_SPEED_SLOW, false);
                game->state = game_init_pos;
            }
            break;

        case game_idle:
            motor_off();
            if (inputs->cmd == game_cmd_init_pos) {
                motor_stopped_counter = 0;
                motor_ctrl_speed(-MOTOR_SPEED_SLOW, false);
                game->state = game_init_pos;
            } else if (inputs->cmd == game_cmd_start_game) {
                motor_stopped_counter = 0;
                game->state = game_start_game;
            }
            break;

        case game_init_pos:
            // Endstop reached
            if (motor_get_current_speed() < MOTOR_SPEED_STOPPED_THRESHOLD) {
                if (++motor_stopped_counter >= GAME_STOPPED_MIN_CYCLES) {
                    motor_off();
                    tc2_qdec_reset();
                    game->state = game_idle;
                }
            } else {
                motor_ctrl_speed(-MOTOR_SPEED_SLOW, false);
            }
            break;

        case game_start_game:
            game->score = 0;
            score_sub_counter = 0;
            pwm_set_dc_servo(CDTY1_MIDDLE);
            motor_init(MOTOR_SPEED_SLOW, MOTOR_CURRENT_LOW);
            motor_ctrl_pos(MOTOR_POS_MIN);
            if (ABS(motor_get_current_position() - MOTOR_POS_MIN) < 5) {
                game->state = game_waiting_for_start;
            } else if (motor_get_current_speed() < MOTOR_SPEED_STOPPED_THRESHOLD) {
                if (++motor_stopped_counter >= GAME_STOPPED_MIN_CYCLES) {
                    motor_off();
                    game->state = game_error;
                }
            }
            break;

        case game_waiting_for_start:
            if (inputs->solenoid_out) {
                solenoid_set_state(true);
                motor_init(MOTOR_SPEED_MAX, MOTOR_CURRENT_MAX);
                game->state = game_active;
            }
            break;

        case game_active:
            motor_ctrl_pos(MOTOR_POS_MIN +
                           ((MOTOR_POS_MAX - MOTOR_POS_MIN) * inputs->pos_joystick) / 255);
            if (++score_sub_counter >= GAME_SCORE_CLK_DIV) {
                score_sub_counter = 0;
                ++game->score;
            }
            if (ir_read() < GAME_IR_THRESHOLD) {
                motor_off();
                solenoid_set_state(false);  // Retract solenoid
                game->state = game_lost;
            }
            break;

        case game_lost:
            if (inputs->cmd == game_cmd_start_game) {
                game->state = game_start_game;
            }
            break;

        case game_error:
            motor_off();
            if (inputs->cmd == game_cmd_reset) {
                game->state = game_idle;
            }
            break;

        default:
            game->state = game_error;
            break;
    }
}
