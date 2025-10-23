/**
 * @file can.c
 * @author author
 * @brief brief
 * @date 2025-10-23
 */

#include "can.h"

#include "../constants.h"
#include "mcp2515.h"
#include "mcp2515_const.h"

#define MCP_TXB0EID8 0x33

#define MCP_TXRTSCTRL 0x0d
#define MCP_BFPCTRL   0x0c

static const uint8_t init_cmds[] = {
    MCP_TXB0CTRL,  0x03,  // priority txb0
    MCP_TXB1CTRL,  0x02,  // priority txb1
    MCP_TXB2CTRL,  0x01,  // priority
    MCP_TXRTSCTRL, 0x00,  // no rts
    MCP_RXB0CTRL,  0x20,  // init read ctrl
    MCP_RXB1CTRL,  0x20,  // init ctrl 1
    MCP_BFPCTRL,   0x00,  // disable rxb-pins
    MCP_CNF1,      0x00,  // TODO find value
    MCP_CNF2,      0x00,  // TODO find value
    MCP_CNF3,      0x00,  // TODO find value
    MCP_EFLG,      0x00,  // TODO find value
    MCP_CANINTE,   0x83,  // TODO check if right
    MCP_CANINTF,   0x00,  // TODO put something here
    MCP_CANCTRL,   0x40,  // Modes
};

void CAN_init() {
    uint8_t length = sizeof(init_cmds) / sizeof(init_cmds[0]);
    uint8_t port;
    uint8_t bit_mask;
    for (uint8_t i = 0; i < length; i += 2) {
        port = init_cmds[i];
        bit_mask = init_cmds[i + 1];
        mcp2515_bit_modify(port, bit_mask, bit_mask);
        delay_us(10);
    }
}

void CAN_send(CAN_DATA* can_data) {
    uint8_t length = 4;

    uint8_t tx_data[4];
    tx_data[0] = can_data->id;
    tx_data[1] = DUMMY;
    tx_data[2] = can_data->data;
    tx_data[3] = can_data->length;

    mcp2515_write(tx_data, MCP_TXB0EID8, length);
    mcp2515_request_to_send(MCP_TXB0EID8);
}

uint8_t CAN_recieve_msg(uint8_t* rx_data, uint8_t address) {
    bool msg_recieved = mcp2515_read(rx_data, address);
    if (!msg_recieved) return 0;  // TODO zero or something ...

    return &rx_data;
}