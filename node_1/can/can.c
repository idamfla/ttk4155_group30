/**
 * @file can.c
 * @author author
 * @brief brief
 * @date 2025-10-23
 */

#include "can.h"

#include "../gpio/gpio.h"

// clang-format off
#include "../constants.h"
#include <util/delay.h>
// clang-format on

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>

#include "mcp2515.h"
#include "mcp2515_const.h"

// #define MCP_TXB0EID8 0x33

#define MCP_TXRTSCTRL 0x0d
#define MCP_BFPCTRL   0x0c
#define MCP_TXB0SIDL  0x32
#define MCP_TXB1SIDL  0x42
#define MCP_TXB2SIDL  0x52
#define MCP_TXB0SIDH  0x31
#define MCP_TXB1SIDH  0x41
#define MCP_TXB2SIDH  0x51

#define EXT_ID_HIGH_VAL DUMMY
#define EXT_ID_LOW_VAL  DUMMY

// MCP_EFLG,      0x00,  // Maybe use in init

static uint8_t init_cmds[] = {
    MCP_CNF1,
    ((1U << 6) | 3U),  // SJW= 2TQ, BRP=3 → TQ = 2 * (3+1)/16MHz = 0.5 µs
    MCP_CNF2,
    ((1U << 7) | (4U << 3) | (7U << 0)),  // BTLMODE=1, PHSEG1=5, PRSEG=8
    MCP_CNF3,
    ((1U << 0)),  // PHSEG2=2
    MCP_CANINTE,
    0x02,  // enable interrupts
};

volatile bool can_int = false;
uint8_t msg_can[10];

static volatile uint8_t rx_data[20];
static volatile uint8_t tx_data[20];

CAN_DATA can_data_recieve;

static void (*_can_rx_cmplt)(CAN_DATA* can_data);

/** @brief Initialize CAN controller */
void CAN_init(void (*can_rx_cmplt)(CAN_DATA* can_data)) {
    _can_rx_cmplt = can_rx_cmplt;
    mcp2515_bit_modify(MCP_CANCTRL, 0xe0, 0x80);  // config mode
    while (!mcp2515_transmit_done());

    uint8_t length = sizeof(init_cmds) / sizeof(init_cmds[0]);
    for (uint8_t i = 0; i < length - 1; i += 2) {
        mcp2515_write(init_cmds + 1 + i, init_cmds[i], 1);
        while (!mcp2515_transmit_done());
    }
    mcp2515_bit_modify(MCP_CANCTRL, 0xe0, 0x00);  // loopback mode 0x40, normal 0x00
    while (!mcp2515_transmit_done());
}

/** @brief Sends data to the can controller and then transmit through the can
 * @param can_data Pointer to CAN_DATA struct containing id, data and length
 */
bool CAN_send(CAN_DATA* can_data) {
    // cli();
    // GICR &= ~(1 << INT1);  // Turning off interrupts while sending CAN message
    // sei();
    if (!mcp2515_transmit_done()) return false;
    uint8_t length = 5 + can_data->length;

    tx_data[0] = (uint8_t)((can_data->id >> 3) & 0xFF);
    tx_data[1] = (uint8_t)((can_data->id << 5) & 0xFF);
    tx_data[2] = EXT_ID_HIGH_VAL;
    tx_data[3] = EXT_ID_LOW_VAL;
    tx_data[4] = can_data->length;

    for (uint8_t i = 0; i < can_data->length; i++) {
        tx_data[5 + i] = can_data->data[i];
    }
    mcp2515_write(tx_data, MCP_TXB0SIDH, length);
    while (!mcp2515_transmit_done());
    mcp2515_request_to_send(MCP_RTS_TX0);
    while (!mcp2515_transmit_done());
    mcp2515_bit_modify(
        MCP_TXB0CTRL, 0x08,
        0x00);  // Clear the TXREQ bit to indicate message has been sent, should not be here
    // cli();
    // GICR |= (1 << INT1);  // Re-enable interrupts
    // sei();
    return true;
}

/** @brief Reads one byte from a register in the can controller
 * @param rx_data Pointer to buffer to store received data
 * @param address Register address to read from
 */
bool CAN_recieve_msg(volatile uint8_t* rx_data, uint8_t address) {
    if (!mcp2515_transmit_done()) return false;
    return mcp2515_read(rx_data, address, 1);
}

/** @brief Sets a flag when RXB1 interrupt is called */
ISR(INT1_vect) {
    can_int = true;
    cli();
    GICR &= ~(1 << INT1);
    sei();
    // mcp2515_bit_modify(MCP_CANINTF, 0xFF, 0x00);
}

/** @brief Sets RXB1 as an external interrupt */
void CAN_setup_interrupt(void) {
    // --- Konfigurer INT1 (PD3) som input ---
    DDRD &= ~(1 << PD3);  // Sett PD3 som input
    PORTD |= (1 << PD3);  // Aktiver pull-up (hvis signalet er åpen kollektor fra MCP2515)

    // --- Velg triggenivå ---
    // MCP2515 INT-linja går lav når en interrupt oppstår → bruk falling edge
    MCUCR |= (1 << ISC11);  // ISC11=1, ISC10=0 → Falling edge
    MCUCR &= ~(1 << ISC10);

    // --- Aktiver ekstern interrupt INT1 ---
    GICR |= (1 << INT1);

    // --- (Valgfritt) tøm eventuelle gamle flagg ---
    GIFR |= (1 << INTF1);
}

/** @brief CAN interrupt handler to read received messages */
void CAN_int_handler(void) {
    if (!mcp2515_transmit_done()) return;
    // mcp2515_bit_modify(MCP_CANINTE, 0x02, 0x00);
    // for (uint8_t i = 0; i < 20; i++) {
    //     rx_data[i] = 0;
    // }
    mcp2515_read(rx_data, MCP_RXB1SIDH, 5);
    while (!mcp2515_transmit_done());
    uint8_t data_length = rx_data[6] & 0xf;
    mcp2515_read(rx_data + 7, 0x76, data_length);
    while (!mcp2515_transmit_done());

    can_data_recieve.id =
        ((uint16_t)(rx_data[2]) << 3) | ((uint16_t)(rx_data[3] & 0b11100000) >> 5);
    can_data_recieve.length = data_length;
    can_data_recieve.data = rx_data + 9;
    _can_rx_cmplt(&can_data_recieve);

    mcp2515_bit_modify(MCP_CANINTF, 0xFF, 0x00);
    while (!mcp2515_transmit_done());
    // mcp2515_bit_modify(MCP_CANINTE, 0x02, 0x02);
    // while (!mcp2515_transmit_done());
    // printf("handler \r\n");
}