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

void pi_init(pi_t* me, int32_t kp, int32_t ki, int32_t T, int32_t output_min, int32_t output_max) {
    me->Kp = kp;
    me->Ki = ki * T;
    me->err_integral = 0;
    me->out_min = output_min;
    me->out_max = output_max;
}

int32_t pi_update(pi_t* me, int32_t reference, int32_t measurement) {
    int32_t error = reference - measurement;
    me->err_integral += error;

    int32_t u = (me->Kp * error) + (me->Ki * me->err_integral);

    if (u < me->out_min) {
        u = me->out_min;
    } else if (u > me->out_max) {
        u = me->out_max;
    }

    return u;
}
