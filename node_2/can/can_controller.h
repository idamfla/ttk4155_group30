/*
 * can_controller.h
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */

#ifndef CAN_CONTROLLER_H_
#define CAN_CONTROLLER_H_

#include <stdint.h>

#define CAN_SMP_ONCE  0
#define CAN_SMP_THREE 1

typedef union {
    uint32_t value;
    struct {
        uint32_t phase_seg2 : 3;
        uint32_t : 1;
        uint32_t phase_seg1 : 3;
        uint32_t : 1;
        uint32_t prop_seg : 3;
        uint32_t : 1;
        uint32_t sjw : 2;
        uint32_t : 2;
        uint32_t brp : 7;
        uint32_t : 1;
        uint32_t smp : 1;
        uint32_t : 7;
    };
} can_br_t;
_Static_assert(sizeof(can_br_t) == sizeof(uint32_t), "can_br_t must be 32 bits");

typedef struct can_message_t {
    uint16_t id;
    char data_length;
    char data[8];
} CAN_MESSAGE;

uint8_t can_init_def_tx_rx_mb(uint32_t can_br);
uint8_t can_init(uint32_t can_br, uint8_t num_tx_mb, uint8_t num_rx_mb);

uint8_t can_send(CAN_MESSAGE* can_msg, uint8_t mailbox_id);
uint8_t can_receive(CAN_MESSAGE* can_msg, uint8_t mailbox_id);

#endif /* CAN_CONTROLLER_H_ */