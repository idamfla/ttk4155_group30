#include <stdarg.h>
#include <stdio.h>

#include "can/can_controller.h"
#include "constants.h"
#include "game/game.h"
#include "ir/ir.h"
#include "pi_controller/motor.h"
#include "pi_controller/pi_controller.h"
#include "pwm/pwm.h"
#include "quad_encoder/quad_encoder.h"
#include "sam.h"
#include "solenoid/solenoid.h"
#include "timer_counter/timer.h"
#include "uart/uart.h"

CAN_MESSAGE msg = {
    .id = 0x1,
    .data_length = 4U,
    .data = {0, 1, 2, 3},
};
volatile game_t game;
game_inputs_t game_inputs = {
    .pos_joystick = 128,
    .pos_slider = 128,
    .cmd = game_cmd_none,
    .solenoid_out = 0,
};
volatile int32_t pos_sp = 0;
volatile motor_state_t motor_state;

void delay_ms(uint32_t ms) {
    SysTick->LOAD = (SystemCoreClock / 1000) - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    for (uint32_t i = 0; i < ms; i++) {
        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
    }
    SysTick->CTRL = 0;
}

void timer_handler(void);

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

    // Enable the peripheral clock for PIOB
    PMC->PMC_PCER0 |= (1U << ID_PIOB);
    PIOB->PIO_PER = PIO_PB17;
    PIOB->PIO_OER = PIO_PB17;
    PIOB->PIO_CODR = PIO_PB17;

    PMC->PMC_PCER0 |= (1U << ID_PIOC);
    PIOC->PIO_PER = PIO_PC23;
    PIOC->PIO_OER = PIO_PC23;
    PIOC->PIO_CODR = PIO_PC23;

    game_init(&game);
    tc0_init(T_MOTOR_CONTROL, timer_handler);

    while (1) {
        pos_sp = MOTOR_POS_MIN;
        delay_ms(2000);
        pos_sp = MOTOR_POS_MAX;
        delay_ms(2000);

        // motor_get_state(&motor_state);
        // // printf("Pos: %ld, Speed: %ld, Curr: %ld\r\n", motor_state.pos_current,
        // //        motor_state.speed_current, motor_state.current_setpoint);
        // printf("Pos: %ld, Pos setpoint: %ld, ", motor_state.pos_current,
        // motor_state.pos_setpoint); printf("Vel: %ld, Vel setpoint: %ld, ",
        // motor_state.speed_current,
        //        motor_state.speed_setpoint);
        // printf("Curr setpoint: %ld\r\n", motor_state.current_setpoint);
    }
}

void timer_handler(void) {
    // game_update(&game, &game_inputs);
    motor_ctrl_pos(pos_sp);
}
