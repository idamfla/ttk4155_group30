/**
 * @file io_board.h
 * @author Marius Bär
 * @brief Header file for io_board.c
 * @date 2025-10-09
 */

#ifndef IO_BOARD_H
#define IO_BOARD_H

#include <stdbool.h>
#include <stdint.h>

typedef struct __attribute__((packed)) {
    uint8_t x;
    uint8_t y;
    uint8_t signal_strength;
} io_touch_pad_t;

typedef struct __attribute__((packed)) {
    uint8_t x;
    uint8_t signal_strength;
} io_touch_slider_t;

typedef struct __attribute__((packed)) {
    uint8_t x;
    uint8_t y;
    bool button;
} io_joystick_t;

typedef struct __attribute__((packed)) {
    union {
        uint8_t left;
        struct {
            uint8_t SL1 : 1;
            uint8_t SL2 : 1;
            uint8_t SL3 : 1;
            uint8_t SL4 : 1;
            uint8_t SL5 : 1;
            uint8_t SL6 : 1;
            uint8_t SL7 : 1;
            uint8_t : 1;
        };
    };
    union {
        uint8_t right;
        struct {
            uint8_t SR1 : 1;
            uint8_t SR2 : 1;
            uint8_t SR3 : 1;
            uint8_t SR4 : 1;
            uint8_t SR5 : 1;
            uint8_t SR6 : 1;
            uint8_t : 2;
        };
    };
    union {
        uint8_t nav;
        struct {
            uint8_t nav_up : 1;
            uint8_t nav_down : 1;
            uint8_t nav_left : 1;
            uint8_t nav_right : 1;
            uint8_t nav_button : 1;
            uint8_t : 3;
        };
    };
} io_buttons_t;

/**
 * @attention Strings are not null-terminated!
 */
typedef union {
    char all[35];
    struct __attribute__((packed)) {
        char firmware_compile_timestamp[19];
        char avr_serial_number[15];
    };
} io_info_t;

typedef struct __attribute__((packed)) {
    // LED index (0-5)
    uint8_t led;
    bool on;
} io_led_on_off_t;

typedef struct __attribute__((packed)) {
    // LED index (0-5)
    uint8_t led;
    // Duty cycle (0-255)
    bool dc;
} io_led_pwm_t;

typedef void (*io_get_touch_pad_cmplt_t)(io_touch_pad_t* touch_pad);
typedef void (*io_get_touch_slider_cmplt_t)(io_touch_slider_t* touch_slider);
typedef void (*io_get_joystick_cmplt_t)(io_joystick_t* joystick);
typedef void (*io_get_buttons_cmplt_t)(io_buttons_t* buttons);
typedef void (*io_get_info_cmplt_t)(io_info_t* info);
typedef void (*io_set_cmplt_t)(void);

bool io_get_touch_pad(io_get_touch_pad_cmplt_t cmplt_cbk);
bool io_get_touch_slider(io_get_touch_slider_cmplt_t cmplt_cbk);
bool io_get_joystick(io_get_joystick_cmplt_t cmplt_cbk);
bool io_get_buttons(io_get_buttons_cmplt_t cmplt_cbk);
bool io_get_info(io_get_info_cmplt_t cmplt_cbk);
bool io_set_led_on_off(io_led_on_off_t* led_state, io_set_cmplt_t cmplt_cbk);
bool io_set_led_pwm(io_led_pwm_t* led_state, io_set_cmplt_t cmplt_cbk);

#endif /* IO_BOARD_H */
