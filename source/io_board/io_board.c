/**
 * @file io_board.c
 * @author Marius Bär
 * @brief IO board communication
 * @date 2025-10-09
 */

#include "io_board.h"

#include <stddef.h>

#include "../spi/spi.h"

typedef enum {
    io_cmd_get_touch_pad = 0x01,
    io_cmd_get_touch_slider = 0x02,
    io_cmd_get_joystick = 0x03,
    io_cmd_get_buttons = 0x04,
    io_cmd_set_leds_on_off = 0x05,
    io_cmd_set_leds_pwm = 0x06,
    io_cmd_get_info = 0x07,
} io_command_t;

typedef enum {
    io_transfer_type_get,
    io_transfer_type_set,
    io_transfer_cmd,
} io_transfer_type_t;

typedef struct {
    union {
        void (*get_cmplt_cbk)(volatile void *data);
        void (*set_cmplt_cbk)(void);
    };
    bool get;
} io_transfer_param_t;

void io_cmd_cmplt(void *unused);
void io_transfer_cmplt(void *unused);

volatile uint8_t _rx_data[35];
uint8_t _tx_data[3];

io_transfer_param_t _transfer_param;

static spi_transfer_t _transfer_cmd = {
    .rx_data = NULL,
    .tx_data = _tx_data,
    .length = 1,
    .slave_idx = spi_slave_io,
    .transfer_cmplt_cbk = io_cmd_cmplt,
};
static spi_transfer_t _transfer_data = {
    .slave_idx = spi_slave_io,
    .transfer_cmplt_cbk = io_transfer_cmplt,
    .param = &_transfer_param,
};
static volatile bool _transfer_active = false;

void io_cmd_cmplt(void *unused) {
    (void)unused;  // unused
    spi_transfer(&_transfer_data);
}

void io_transfer_cmplt(void *unused) {
    (void)unused;  // unused
    if (_transfer_param.get) {
        _transfer_param.get_cmplt_cbk(_transfer_data.rx_data);
    } else {
        _transfer_param.set_cmplt_cbk();
    }
    _transfer_active = false;
}

/**
 * @brief Get the state of the touch pad
 * @param cmplt_cbk Callback function called when the touch pad state is received
 */
void io_get_touch_pad(io_get_touch_pad_cmplt_t cmplt_cbk) {
    _tx_data[0] = io_cmd_get_touch_pad;

    _transfer_data.tx_data = NULL;
    _transfer_data.rx_data = _rx_data;
    _transfer_data.length = sizeof(io_touch_pad_t);

    _transfer_param.get = true;
    _transfer_param.get_cmplt_cbk = (void (*)(volatile void *))cmplt_cbk;

    _transfer_active = true;
    spi_transfer(&_transfer_cmd);
}

/**
 * @brief Get the state of the touch slider
 * @param cmplt_cbk Callback function called when the touch slider state is received
 */
void io_get_touch_slider(io_get_touch_slider_cmplt_t cmplt_cbk) {
    (void)cmplt_cbk; /* unused parameter */
}

/**
 * @brief Get the state of the joystick
 * @param cmplt_cbk Callback function called when the joystick state is received
 */
void io_get_joystick(io_get_joystick_cmplt_t cmplt_cbk) {
    (void)cmplt_cbk; /* unused parameter */
}

/**
 * @brief Get the state of the buttons
 * @param cmplt_cbk Callback function called when the button states are received
 */
void io_get_buttons(io_get_buttons_cmplt_t cmplt_cbk) {
    _tx_data[0] = io_cmd_get_buttons;

    _transfer_data.tx_data = NULL;
    _transfer_data.rx_data = _rx_data;
    _transfer_data.length = sizeof(io_buttons_t);

    _transfer_param.get = true;
    _transfer_param.get_cmplt_cbk = (void (*)(volatile void *))cmplt_cbk;

    _transfer_active = true;
    spi_transfer(&_transfer_cmd);
    (void)cmplt_cbk; /* unused parameter */
}

/**
 * @brief Get general information about the IO board
 * @param cmplt_cbk Callback function called when the information is received
 */
void io_get_info(io_get_info_cmplt_t cmplt_cbk) {
    (void)cmplt_cbk; /* unused parameter */
}

/**
 * @brief Set the specified LED on or off
 * @param led LED index (0-5)
 * @param on true to turn on, false to turn off
 */
void io_set_leds_on_off(io_led_on_off_t *led_state) {
    (void)led_state; /* unused parameter */
}

/**
 * @brief Set the duty cycle of the specified LED
 * @param led LED index (0-5)
 * @param dc Duty cycle (0-255)
 */
void io_set_leds_pwm(io_led_pwm_t *led_state) {
    (void)led_state; /* unused parameter */
}
