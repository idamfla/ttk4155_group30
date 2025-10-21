/**
 * @file io_board.c
 * @author Marius Bär
 * @brief IO board communication
 * @date 2025-10-09
 */

// clang-format off
#include "../constants.h"
#include <util/delay.h>
// clang-format on

#include "io_board.h"

#include <stddef.h>

#include "../spi/spi.h"

typedef enum {
    io_cmd_get_touch_pad = 0x01,
    io_cmd_get_touch_slider = 0x02,
    io_cmd_get_joystick = 0x03,
    io_cmd_get_buttons = 0x04,
    io_cmd_set_led_on_off = 0x05,
    io_cmd_set_led_pwm = 0x06,
    io_cmd_get_info = 0x07,
} io_command_t;

// typedef enum {
//     io_transfer_type_get,
//     io_transfer_type_set,
//     io_transfer_cmd,
// } io_transfer_type_t;

typedef struct {
    union {
        void (*get_cmplt_cbk)(volatile void* data);
        void (*set_cmplt_cbk)(void);
    };
    bool get;
} io_transfer_param_t;

void io_spi_start_cbk(void);
void io_transfer_cmplt(void* unused);

volatile uint8_t _rx_data[35];
volatile io_command_t _cmd;
io_transfer_param_t _transfer_cmplt_param;
static volatile bool _transfer_active = false;

static spi_transfer_t _transfer_data = {
    .slave_idx = spi_slave_io,
    .transfer_cmplt_cbk = io_transfer_cmplt,
    .cmplt_param = &_transfer_cmplt_param,
    .transfer_start_cbk = io_spi_start_cbk,
};

void io_spi_start_cbk(void) {
    spi_ll_transmit_blocking((uint8_t)_cmd);
    _delay_us(24);
}

void io_transfer_cmplt(void* unused) {
    (void)unused;  // unused
    if (_transfer_cmplt_param.get) {
        _transfer_cmplt_param.get_cmplt_cbk(_transfer_data.rx_data);
    } else {
        if (_transfer_cmplt_param.set_cmplt_cbk) {
            _transfer_cmplt_param.set_cmplt_cbk();
        }
    }
    _transfer_active = false;
}

/**
 * @brief Get the state of the touch pad
 * @param cmplt_cbk Callback function called when the touch pad state is received (cannot be NULL)
 * @retval true if the command was sent
 * @retval false if a transfer was already active and the new command could not be sent
 */
bool io_get_touch_pad(io_get_touch_pad_cmplt_t cmplt_cbk) {
    if (_transfer_active) {
        return false;
    }
    _transfer_active = true;
    _cmd = io_cmd_get_touch_pad;

    _transfer_data.tx_data = NULL;
    _transfer_data.rx_data = _rx_data;
    _transfer_data.length = sizeof(io_touch_pad_t);

    _transfer_cmplt_param.get = true;
    _transfer_cmplt_param.get_cmplt_cbk = (void (*)(volatile void*))cmplt_cbk;

    return spi_transfer(&_transfer_data);
}

/**
 * @brief Get the state of the touch slider
 * @param cmplt_cbk Callback function called when the touch slider state is received (cannot be
 * NULL)
 * @retval true if the command was sent
 * @retval false if a transfer was already active and the new command could not be sent
 */
bool io_get_touch_slider(io_get_touch_slider_cmplt_t cmplt_cbk) {
    if (_transfer_active) {
        return false;
    }
    _transfer_active = true;
    _cmd = io_cmd_get_touch_slider;

    _transfer_data.tx_data = NULL;
    _transfer_data.rx_data = _rx_data;
    _transfer_data.length = sizeof(io_touch_slider_t);

    _transfer_cmplt_param.get = true;
    _transfer_cmplt_param.get_cmplt_cbk = (void (*)(volatile void*))cmplt_cbk;

    return spi_transfer(&_transfer_data);
}

/**
 * @brief Get the state of the joystick
 * @param cmplt_cbk Callback function called when the joystick state is received (cannot be NULL)
 * @retval true if the command was sent
 * @retval false if a transfer was already active and the new command could not be sent
 */
bool io_get_joystick(io_get_joystick_cmplt_t cmplt_cbk) {
    if (_transfer_active) {
        return false;
    }
    _transfer_active = true;
    _cmd = io_cmd_get_joystick;

    _transfer_data.tx_data = NULL;
    _transfer_data.rx_data = _rx_data;
    _transfer_data.length = sizeof(io_joystick_t);

    _transfer_cmplt_param.get = true;
    _transfer_cmplt_param.get_cmplt_cbk = (void (*)(volatile void*))cmplt_cbk;

    return spi_transfer(&_transfer_data);
}

/**
 * @brief Get the state of the buttons
 * @param cmplt_cbk Callback function called when the button states are received (cannot be NULL)
 * @retval true if the command was sent
 * @retval false if a transfer was already active and the new command could not be sent
 */
bool io_get_buttons(io_get_buttons_cmplt_t cmplt_cbk) {
    if (_transfer_active) {
        return false;
    }
    _transfer_active = true;
    _cmd = io_cmd_get_buttons;

    _transfer_data.tx_data = NULL;
    _transfer_data.rx_data = _rx_data;
    _transfer_data.length = sizeof(io_buttons_t);

    _transfer_cmplt_param.get = true;
    _transfer_cmplt_param.get_cmplt_cbk = (void (*)(volatile void*))cmplt_cbk;

    return spi_transfer(&_transfer_data);
}

/**
 * @brief Get general information about the IO board
 * @param cmplt_cbk Callback function called when the information is received (cannot be NULL)
 * @retval true if the command was sent
 * @retval false if a transfer was already active and the new command could not be sent
 */
bool io_get_info(io_get_info_cmplt_t cmplt_cbk) {
    if (_transfer_active) {
        return false;
    }
    _transfer_active = true;
    _cmd = io_cmd_get_info;

    _transfer_data.tx_data = NULL;
    _transfer_data.rx_data = _rx_data;
    _transfer_data.length = sizeof(io_info_t);

    _transfer_cmplt_param.get = true;
    _transfer_cmplt_param.get_cmplt_cbk = (void (*)(volatile void*))cmplt_cbk;

    return spi_transfer(&_transfer_data);
}

/**
 * @brief Set the specified LED on or off
 * @param led_state Pointer to the LED state structure
 * @param cmplt_cbk Callback function called when the command was sent
 * @retval true if the command was sent
 * @retval false if a transfer was already active and the new command could not be sent
 */
bool io_set_led_on_off(io_led_on_off_t* led_state, io_set_cmplt_t cmplt_cbk) {
    if (_transfer_active) {
        return false;
    }
    _transfer_active = true;
    _cmd = io_cmd_set_led_on_off;

    _transfer_data.tx_data = (uint8_t*)led_state;
    _transfer_data.rx_data = NULL;
    _transfer_data.length = sizeof(io_led_on_off_t);

    _transfer_cmplt_param.get = false;
    _transfer_cmplt_param.set_cmplt_cbk = (void (*)(void))cmplt_cbk;

    return spi_transfer(&_transfer_data);
}

/**
 * @brief Set the duty cycle of the specified LED
 * @param led_state Pointer to the LED PWM state structure
 * @param cmplt_cbk Callback function called when the command was sent
 * @retval true if the command was sent
 * @retval false if a transfer was already active and the new command could not be sent
 */
bool io_set_led_pwm(io_led_pwm_t* led_state, io_set_cmplt_t cmplt_cbk) {
    if (_transfer_active) {
        return false;
    }
    _transfer_active = true;
    _cmd = io_cmd_set_led_pwm;

    _transfer_data.tx_data = (uint8_t*)led_state;
    _transfer_data.rx_data = NULL;
    _transfer_data.length = sizeof(io_led_pwm_t);

    _transfer_cmplt_param.get = false;
    _transfer_cmplt_param.set_cmplt_cbk = (void (*)(void))cmplt_cbk;

    return spi_transfer(&_transfer_data);
}
