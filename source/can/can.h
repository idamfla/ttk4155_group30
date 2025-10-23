/**
 * @file can.h
 * @author Marius Bär
 * @brief Header file for can.c
 * @date 2025-10-23
 */

#ifndef CAN_H
#define CAN_H

// Can init function to configure sample point, bit rate, interrupts, filters and masks

// Implement can controller as state machine
// Init state is the reset state
// next state is the configuration state
// idle state (waiting for interrupts or messages to send)
// transmit state
// - write transmit buffer (TXBnDm registers)
// - set DLC (TXBnDLC register)
// - set message ID (TXBnSIDH/TXBnSIDL registers)
// - set request to send (TXBnCTRL register)
// receive state
//

// What interface makes sense for sending and receiving can messages?

#endif /* CAN_H */
