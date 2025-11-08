#include <stdarg.h>
#include <stdio.h>

#include "can/can_controller.h"
#include "constants.h"
#include "pwm/pwm.h"
#include "sam.h"
#include "uart/uart.h"

CAN_MESSAGE msg = {
    .id = 0x1,
    .data_length = 4U,
    .data = {0, 1, 2, 3},
};

int main() {
    SystemInit();

    // Disable Watchdog Timer
    WDT->WDT_MR = WDT_MR_WDDIS;

    uart_init(F_CPU, UART_BAUD);
    printf("Hello World\r\n");

    can_br_t can_br = {
        .sjw = (CAN_SJW - 1U),
        .prop_seg = (CAN_PROP_SEG - 1U),
        .phase_seg1 = (CAN_PHASE_SEG1 - 1U),
        .phase_seg2 = (CAN_PHASE_SEG2 - 1U),
        .brp = (F_CPU / F_CAN / CAN_TQ),
        .smp = CAN_SMP_ONCE,
    };
    can_init_def_tx_rx_mb(can_br.value);

    can_send(&msg, 0U);

    pwm_init();

    while (1) {
        /* code */
    }
}
