/**
 * @file can.c
 * @author author
 * @brief brief
 * @date 2025-10-09
 */

#include "can.h"

#include "mcp2515.h"
#include "spi.h"

#define PORT_MCP2515_RST 0  // TODO find a nice place for this
#define PIN_MCP2515_RST  0  // TODO find a nice place for this
#define CAN_CS           0  // TODO find a nice place for this

/** @brief Initialize the MCP2515 */
void mcp2515_init() {
    uint8_t value;

    spi_master_init();
    mcp2515_reset();  // Send reset-command

    // Self test
    value = mcp2515_read(MCP_CANSTAT);
    if ((value & MODE_MASK) != MODE_CONFIG) {
        printf("MCP2515 is NOT in configuration mode after reset!\n");
        return;
    }
}

/**
 * @brief does some reading i think
 * @param address i think this is the address we want to read from
 */
uint8_t mcp2515_read(uint8_t address) {
    (void)0;  // TODO implement variables and stuff
    uint8_t result;
    PORTB &= ~(1 << CAN_CS);

    spi_write(MCP_READ);
    spi_write(address);
    result = spi_read();

    PORTB |= (1 << CAN_CS);
    return result;
}

/** @brief Resets the OLED display */
void mcp2515_reset(void) {
    CLEAR_PIN(PORT_MCP2515_RST, PIN_MCP2515_RST);
    _delay_us(10);
    SET_PIN(PORT_MCP2515_RST, PIN_MCP2515_RST);
    _delay_us(10);
}