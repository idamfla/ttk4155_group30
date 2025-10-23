#include <stdarg.h>
#include <stdio.h>

#include "constants.h"
#include "sam.h"
#include "uart/uart.h"

int main() {
    SystemInit();

    // Disable Watchdog Timer
    WDT->WDT_MR = WDT_MR_WDDIS;

    uart_init(F_CPU, UART_BAUD);
    printf("Hello World\r\n");

    while (1) {
        /* code */
    }
}
