/**
 * @file can.c
 * @author author
 * @brief brief
 * @date 2025-10-09
 */

#include "can.h"
// clang-format off
#include "../constants.h"
// clang-format on

#include <util/delay.h>

#include "../gpio/gpio.h"
#include "../spi/spi.h"
#include "../oled/oled.h"
#include "mcp2515.h"

#define DDR_CAN_CS  DDRB   // TODO find place
#define PORT_CAN_CS PORTB  // TODO find place
#define PIN_CAN_CS  PIN0   // TODO find place

#define DDR_CAN_INTERRUPT  DDRB   // TODO find place
#define PORT_CAN_INTERRUPT PORTB  // TODO find place
#define PIN_CAN_INTERRUPT  PIN0   // TODO find place

/** @brief Initialize the MCP2515 */
void can_init() {
    spi_master_init();
    can_reset();  // Send reset-command

    // Self test
    uint8_t value;
    value = can_read(MCP_CANSTAT);
    if ((value & MODE_MASK) != MODE_CONFIG) {
        printf("CAN (MCP2515) is NOT in configuration mode after reset!\n");
        return;
    }
}

/**
 * @brief does some reading i think
 * @param address i think this is the address we want to read from
 */
uint8_t can_read(uint8_t address) {
    (void)0;  // TODO implement variables and stuff
    uint8_t result;
    PORTB &= ~(1 << PIN_CAN_CS);

    // spi_write(MCP_READ);
    // spi_write(address);
    // result = spi_read();

    PORTB |= (1 << PIN_CAN_CS);
    return address;  // TODO remove later
    // return result;
}

/** @brief Resets the OLED display */
void can_reset(void) {
    oled_reset();
}