/**
 * @file io_board.c
 * @author Marius Bär
 * @brief IO board communication
 * @date 2025-10-09
 */

#include "io_board.h"

#include "../spi/spi.h"

typedef struct {
    union {
        void (*get_cmplt_cbk)(volatile void *data);
        void (*set_cmplt_cbk)(void);
    };
    bool get;  // Whether it's a get (true) or set (false) operation
} io_transfer_cmplt_t;

void io_transfer_cmplt(io_transfer_cmplt_t *param);

volatile uint8_t _rx_data[35];
volatile uint8_t _tx_data[3];
static volatile spi_transfer_t _transfer = {
    .rx_data = _rx_data,
    .slave_idx = spi_slave_io,
    .transfer_cmplt_cbk = (void (*)(void *))io_transfer_cmplt};
static volatile bool _transfer_active = false;

void io_transfer_cmplt(io_transfer_cmplt_t *param) {
    _transfer_active = false;
    if (param->get) {
        param->get_cmplt_cbk(_transfer.rx_data);
    } else {
        param->set_cmplt_cbk();
    }
}

/**
 * @brief Get the state of the touch pad
 * @param cmplt_cbk Callback function called when the touch pad state is received
 */
void io_get_touch_pad(io_get_touch_pad_cmplt_t cmplt_cbk) {
    (void)cmplt_cbk; /* unused parameter */
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
