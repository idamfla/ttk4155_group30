/**
 * @file can.c
 * @author author
 * @brief brief
 * @date 2025-10-23
 */

#include "can.h"

#include "mcp2515.h"

// void CAN_send(CAN_DATA* can_data) {
//     uint8_t tx_data[2];
//     tx_data[0] = can_data->id;
//     tx_data[1] = can_data->data;

//     uint8_t address = can_data->address;
//     uint8_t length = can_data->length + 1;

//     mcp2515_write(tx_data, address, length);
//     mcp2515_request_to_send(address);
// }

// CAN_DATA CAN_recieve_msg(uint8_t* rx_data, uint8_t address) {
//     if (!mcp2515_read(rx_data, address)) return;

//     mcp
// }