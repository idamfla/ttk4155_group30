#ifndef CONSTANTS_H
#define CONSTANTS_H

#define F_CPU     84U * 1000U * 1000U  // CPU frequency in Hz
#define UART_BAUD 38400U               // UART baud rate

#define F_CAN          (250U * 1000U)
#define CAN_SJW        2U
#define CAN_PROP_SEG   8U
#define CAN_PHASE_SEG1 5U
#define CAN_PHASE_SEG2 2U
#define CAN_TQ         (1U /* Sync Seg*/ + CAN_PROP_SEG + CAN_PHASE_SEG1 + CAN_PHASE_SEG2)
#define CAN_BRP        (F_CPU / F_CAN / CAN_TQ - 1U)

_Static_assert(CAN_TQ == 16, "CAN_TQ must be 16");
_Static_assert(CAN_BRP > 0, "CAN_BRP cannot be 0");

#endif  // CONSTANTS_H
