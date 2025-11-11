#include <stdarg.h>
#include <stdio.h>

#include "can/can_controller.h"
#include "constants.h"
#include "ir/ir.h"
#include "pwm/pwm.h"
#include "quad_encoder/quad_encoder.h"
#include "sam.h"
#include "solenoid/solenoid.h"
#include "uart/uart.h"

CAN_MESSAGE msg = {
    .id = 0x1,
    .data_length = 4U,
    .data = {0, 1, 2, 3},
};

#include "sam.h"

void delay_ms(uint32_t ms) {
    SysTick->LOAD = (SystemCoreClock / 1000) - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    for (uint32_t i = 0; i < ms; i++) {
        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
    }
    SysTick->CTRL = 0;
}

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
    ir_init();

    pwm_init();
    solenoid_init();
    tc2_qdec_init();

    while (1) {
        pwm_set_dc(CDTY_MAX);
        // uint16_t ir_adc = ir_read();
        // printf("IR ADC Value: %u\r\n", ir_adc);
        tc2_qdec_reset();
        delay_ms(100);
        int32_t pos = tc2_qdec_get_position();
        printf("Encoder Position: %ld\r\n", pos);
    }
}
