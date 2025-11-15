/**
 * @file motor.h
 * @author Marius Bär
 * @brief Motor control
 * @date 2025-11-15
 */

#include "motor.h"

#include "constants.h"
#include "pi_controller/pi_controller.h"
#include "pwm/pwm.h"
#include "quad_encoder/quad_encoder.h"

#define ABS(x)      ((x) < 0 ? -(x) : (x))
#define POS_SHIFT   4U
#define SPEED_SHIFT 20U
#define CUR_SHIFT   16U

static pi_t _pi_speed;
static pi_t _pi_pos;

static volatile int32_t _pos_prev;
static volatile int32_t _pos_setpoint;  // Used for velocity control with position loop
static volatile int32_t _speed_setpoint;
static volatile int32_t _current_setpoint;
static volatile int32_t _pos_current;
static volatile int32_t _speed_current;

void motor_get_state(motor_state_t* state) {
    state->pos_prev = _pos_prev;
    state->pos_setpoint = _pos_setpoint;
    state->speed_setpoint = _speed_setpoint;
    state->current_setpoint = _current_setpoint;
    state->pos_current = _pos_current;
    state->speed_current = _speed_current;
}

void motor_off(void) {
    pwm_set_dc_motor(0);
}

void motor_init(int32_t speed_max, int32_t current_max) {
    if (speed_max > MOTOR_SPEED_MAX) {
        speed_max = MOTOR_SPEED_MAX;
    }
    if (current_max > CPRD0) {
        current_max = CPRD0;
    }
    pi_init(&_pi_pos, MOTOR_POS_KP, MOTOR_POS_KI, T_MOTOR_CONTROL, -speed_max << SPEED_SHIFT,
            speed_max << SPEED_SHIFT);
    pi_init(&_pi_speed, MOTOR_SPEED_KP, MOTOR_SPEED_KI, T_MOTOR_CONTROL, -current_max << CUR_SHIFT,
            current_max << CUR_SHIFT);

    pwm_set_dc_motor(0);
    _pos_setpoint = tc2_qdec_get_position();
    _pos_prev = _pos_setpoint;
}

void motor_ctrl_pos(int32_t pos_setpoint) {
    _pos_setpoint = pos_setpoint;

    _pos_current = tc2_qdec_get_position();
    _speed_current = (_pos_current - _pos_prev);
    _speed_setpoint = pi_update(&_pi_pos, _pos_setpoint, _pos_current) >> SPEED_SHIFT;
    _current_setpoint = pi_update(&_pi_speed, _speed_setpoint, _speed_current) >> CUR_SHIFT;

    pwm_set_dc_motor(ABS(_current_setpoint));
    if (_current_setpoint >= 0) {
        PIOC->PIO_CODR = PIO_PC23;  // DIR pin low
    } else {
        PIOC->PIO_SODR = PIO_PC23;  // DIR pin high
    }

    _pos_prev = _pos_current;
}

void motor_ctrl_speed(int32_t speed_setpoint, bool with_pos_loop) {
    _pos_current = tc2_qdec_get_position();
    _speed_current = (_pos_current - _pos_prev);

    if (with_pos_loop) {
        _pos_setpoint += speed_setpoint;
        _speed_setpoint = pi_update(&_pi_pos, _pos_setpoint, _pos_current) >> SPEED_SHIFT;
    } else {
        _speed_setpoint = speed_setpoint << SPEED_SHIFT;
    }
    _current_setpoint = pi_update(&_pi_speed, _speed_setpoint, _speed_current) >> CUR_SHIFT;

    pwm_set_dc_motor(ABS(_current_setpoint));
    if (_current_setpoint >= 0) {
        PIOC->PIO_CODR = PIO_PC23;  // DIR pin low
    } else {
        PIOC->PIO_SODR = PIO_PC23;  // DIR pin high
    }

    _pos_prev = _pos_current;
}
