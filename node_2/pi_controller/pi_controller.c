/**
 * @file pi_controller.c
 * @author author
 * @brief brief
 * @date 2025-11-09
 */

#include "pi_controller.h"
// #define PI_OUTPUT_MAX
// #define PI_OUTPUT_MIN
#define SAMPLE_TIME_MS 10

static PI pi;

void pi_init(uint8_t kp, uint8_t ki, uint8_t output_min, uint8_t output_max) {
    pi.Kp = kp;
    pi.Ki = ki;
    pi.integrator = 0;
    pi.past_errors = 0;
    pi.T = SAMPLE_TIME_MS;
    pi.out_min = output_min;
    pi.out_max = output_max;
}

uint8_t pi_update(uint8_t reference, uint8_t measurement) {
    uint8_t error = reference - measurement;
    pi.past_errors += error;

    uint8_t delta_u = (pi.Kp * error) + (pi.T * pi.Ki * pi.past_errors);

    pi.integrator += delta_u;
    if (pi.integrator < pi.out_min) {
        pi.integrator = pi.out_min;
    } else if (pi.integrator > pi.out_max) {
        pi.integrator = pi.out_max;
    }
    return pi.integrator;
}