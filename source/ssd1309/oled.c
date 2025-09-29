/**
 * @file spi.h
 * @author Thomas Berg
 * @author Ida Flakstad
 * @brief brief
 * @date 2025-09-29
 */


#include "oled.h"
#include <util/delay.h>
#include <stdint.h>

void oled_send_command(uint8_t cmd){
    //Turn on command mode, and select slave
    PORT_OLED_DC &= ~(1 << PIN_OLED_DC);
    PORT_OLED_SS &= ~(1 << PIN_OLED_SS);
    //Send command
    spi_transfer_t transfer = {
        .data = &cmd,
        .length = 1,
        .slave = spi_slave_disp_c,
    };
    spi_transfer(&transfer);
    //Deselect slave and turn off command mode
    PORT_OLED_SS |= (1 << PIN_OLED_SS);
    PORT_OLED_DC |= (1 << PIN_OLED_DC);
}

void oled_reset(void){
    PORT_OLED_RST &= ~(1 << PIN_OLED_RST);
    _delay_ms(10);
    PORT_OLED_RST |= (1 << PIN_OLED_RST);
    _delay_ms(10);
}

void oled_init(void){
    // Set pins as output
    SET_PIN(DDR_OLED_DC, PIN_OLED_DC);
    SET_PIN(DDR_OLED_SS, PIN_OLED_SS);
    SET_PIN(DDR_OLED_RST, PIN_OLED_RST);

    // Deselect slave
    SET_PIN(PORT_OLED_SS, PIN_OLED_SS);
    oled_reset();

    // send init commands
    for (uint8_t i = 0; i < sizeof(init_cmds); i++){
        oled_send_command(init_cmds[i]);
    }
}

