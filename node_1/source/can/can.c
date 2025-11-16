/**
 * @file can.c
 * @author Marius Bär
 * @brief High level CAN driver using the MCP2515
 * @date 2025-11-16
 */

#include "can.h"

#include <stdio.h>
#include <string.h>

#include "../interrupt.h"
#include "mcp2515.h"

#define INIT_CMD_COUNT  (sizeof(init_cmds) / sizeof(init_cmds[0]) / 2)
#define BUFFER_SIZE     10U
#define EXT_ID_HIGH_VAL 0U  // Dummy
#define EXT_ID_LOW_VAL  0U  // Dummy

static const uint8_t init_cmds[] = {
    MCP_CNF1,
    ((1U << 6) | 3U),  // SJW= 2TQ, BRP=3 → TQ = 2 * (3+1)/16MHz = 0.5 µs
    MCP_CNF2,
    ((1U << 7) | (4U << 3) | (7U << 0)),  // BTLMODE=1, PHSEG1=5, PRSEG=8
    MCP_CNF3,
    ((1U << 0)),  // PHSEG2=2
    MCP_CANINTE,
    0x02,  // enable interrupts
};

static uint8_t _tx_data[BUFFER_SIZE];
static const can_message_t* volatile _can_msg;

static volatile can_state_t _can_state;
static volatile uint8_t init_cmd_counter;

void mcp2515_write_cmplt(void);
void mcp2515_read_cmplt(volatile uint8_t* data);
void mcp2515_bit_modify_cmplt(void);
void mcp2515_read_status_cmplt(volatile uint8_t* status);
void mcp2515_reset_cmplt(void);
void mcp2515_rts_cmplt(void);

bool can_init(void) {
    _can_state = can_state_wait_for_reset;
    if (mcp2515_reset(mcp2515_reset_cmplt)) {
        return true;
    } else {
        return false;
    }
}

void can_update(can_event_t event) {
    uint8_t sreg = INTERRUPT_DISABLE();
    switch (_can_state) {
        case can_state_wait_for_reset:
            if (event == can_event_mcp2515_reset_done) {
                init_cmd_counter = 0;
                mcp2515_bit_modify(MCP_CANCTRL, 0xe0, 0x80,
                                   mcp2515_bit_modify_cmplt);  // config mode
                _can_state = can_state_wait_config_mode;
            }
            break;

        case can_state_wait_config_mode:
            if (event == can_event_mcp2515_bit_modify_done) {
                _tx_data[2] = init_cmds[2 * init_cmd_counter + 1];
                mcp2515_write(_tx_data, init_cmds[2 * init_cmd_counter], 1, mcp2515_write_cmplt);
                ++init_cmd_counter;
                _can_state = can_state_wait_write_config;
            }
            break;

        case can_state_wait_write_config:
            if (event == can_event_mcp2515_write_done) {
                if (init_cmd_counter < INIT_CMD_COUNT) {
                    _tx_data[2] = init_cmds[2 * init_cmd_counter + 1];
                    mcp2515_write(_tx_data, init_cmds[2 * init_cmd_counter], 1,
                                  mcp2515_write_cmplt);
                    ++init_cmd_counter;
                } else {
                    mcp2515_bit_modify(MCP_CANCTRL, 0xe0, 0x00,
                                       mcp2515_bit_modify_cmplt);  // normal mode
                    _can_state = can_state_wait_normal_mode;
                }
            }
            break;

        case can_state_wait_normal_mode:
            if (event == can_event_mcp2515_bit_modify_done) {
                _can_state = can_state_idle;
            }
            break;

        case can_state_idle:
            if (event == can_event_tx_start) {
                _tx_data[2] = (uint8_t)((_can_msg->id >> 3) & 0xFF);
                _tx_data[3] = (uint8_t)((_can_msg->id << 5) & 0xFF);
                _tx_data[4] = EXT_ID_HIGH_VAL;
                _tx_data[5] = EXT_ID_LOW_VAL;
                _tx_data[6] = _can_msg->length;
                memcpy(&_tx_data[7], _can_msg->data, _can_msg->length);
                mcp2515_write(_tx_data, MCP_TXB0SIDH, _can_msg->length + 5, mcp2515_write_cmplt);
                _can_state = can_state_tx_prepare_data;
            }
            break;

        case can_state_tx_prepare_data:
            if (event == can_event_mcp2515_write_done) {
                mcp2515_request_to_send(MCP_RTS_TX0, mcp2515_rts_cmplt);
                _can_state = can_state_wait_rts_done;
            }
            break;

        case can_state_wait_rts_done:
            if (event == can_event_rts_done) {
                _can_state = can_state_idle;
            }
            break;

        case can_state_transmitting:
            break;

        default:
            _can_state = can_state_wait_for_reset;
            break;
    }
    INTERRUPT_RESTORE(sreg);
}

bool can_send(const can_message_t* can_msg) {
    if (can_msg->length > (BUFFER_SIZE - 7U)) {
        return false;
    }
    uint8_t sreg = INTERRUPT_DISABLE();
    if (_can_state != can_state_idle) {
        INTERRUPT_RESTORE(sreg);
        return false;
    }
    _can_msg = can_msg;
    can_update(can_event_tx_start);
    INTERRUPT_RESTORE(sreg);
    return true;
}

can_state_t can_get_state(void) {
    return _can_state;
}

void mcp2515_write_cmplt(void) {
    can_update(can_event_mcp2515_write_done);
}
void mcp2515_read_cmplt(volatile uint8_t* data) {
    (void)data;
}
void mcp2515_bit_modify_cmplt(void) {
    can_update(can_event_mcp2515_bit_modify_done);
}
void mcp2515_read_status_cmplt(volatile uint8_t* status) {
    (void)status;
}

void mcp2515_reset_cmplt(void) {
    can_update(can_event_mcp2515_reset_done);
}

void mcp2515_rts_cmplt(void) {
    can_update(can_event_rts_done);
}
