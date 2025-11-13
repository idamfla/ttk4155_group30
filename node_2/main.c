#include <stdarg.h>
#include <stdio.h>

#include "can/can_controller.h"
#include "constants.h"
#include "ir/ir.h"
#include "pi_controller/pi_controller.h"
#include "pwm/pwm.h"
#include "quad_encoder/quad_encoder.h"
#include "sam.h"
#include "solenoid/solenoid.h"
#include "timer_counter/timer.h"
#include "uart/uart.h"

#define ABS(x)      ((x) < 0 ? -(x) : (x))
#define POS_SHIFT   4U
#define SPEED_SHIFT 20U
#define CUR_SHIFT   16U

CAN_MESSAGE msg = {
    .id = 0x1,
    .data_length = 4U,
    .data = {0, 1, 2, 3},
};

static pi_t _pi_speed;
static pi_t _pi_pos;

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

static volatile int32_t position = 0;
static volatile int32_t speed = 0;
static volatile int32_t current = 0;
static volatile int32_t position_sp = 0;
static volatile int32_t speed_sp = 0;
static volatile int32_t pos_sp = 0;

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

    pi_init(&_pi_pos, 320000, 0, T_MOTOR_CONTROL, -150 << SPEED_SHIFT, 150 << SPEED_SHIFT);
    pi_init(&_pi_speed, 12000000, 100000, T_MOTOR_CONTROL, -((int32_t)CPRD0) << CUR_SHIFT,
            CPRD0 << CUR_SHIFT);

    // Enable the peripheral clock for PIOB
    PMC->PMC_PCER0 |= (1U << ID_PIOB);
    PIOB->PIO_PER = PIO_PB17;
    PIOB->PIO_OER = PIO_PB17;
    PIOB->PIO_CODR = PIO_PB17;

    PMC->PMC_PCER0 |= (1U << ID_PIOC);
    PIOC->PIO_PER = PIO_PC23;
    PIOC->PIO_OER = PIO_PC23;
    PIOC->PIO_CODR = PIO_PC23;

    tc0_init(T_MOTOR_CONTROL, timer_handler);

    while (1) {
        pos_sp = 200;
        delay_ms(500);
        pos_sp = 5600 / 2;
        delay_ms(500);
        pos_sp = 5600 - 200;
        delay_ms(500);

        pwm_set_dc_servo(CDTY1_MAX);
        printf("Position: %ld, Position SP: %ld, Speed: %ld, Speed SP: %ld, Current: %ld\r\n",
               position, position_sp, speed, speed_sp, current);
    }
}

void timer_handler(void) {
    static int32_t pos_prev;
    int32_t pos_setpoint = pos_sp;
    int32_t speed_setpoint;
    int32_t current_setpoint;

    int32_t pos_current;
    int32_t speed_current;

    pos_current = tc2_qdec_get_position();
    speed_current = (pos_current - pos_prev);

    speed_setpoint = pi_update(&_pi_pos, pos_setpoint, pos_current) >> SPEED_SHIFT;
    current_setpoint = pi_update(&_pi_speed, speed_setpoint, speed_current) >> CUR_SHIFT;

    pwm_set_dc_motor(ABS(current_setpoint));
    if (current_setpoint >= 0) {
        PIOC->PIO_CODR = PIO_PC23;  // DIR pin low
    } else {
        PIOC->PIO_SODR = PIO_PC23;  // DIR pin high
    }

    position = pos_current;
    speed = speed_current;
    current = current_setpoint;
    position_sp = pos_setpoint;
    speed_sp = speed_setpoint;

    pos_prev = pos_current;
}
