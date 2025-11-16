/**
 * @file interrupt.h
 * @author Marius Bär
 * @brief AVR interrupt.h with additional macro to and restore the global interrupt state
 * @date 2025-11-16
 */

#pragma once

#include <avr/interrupt.h>

#define INTERRUPT_DISABLE() \
    ({                      \
        uint8_t __s = SREG; \
        cli();              \
        __s;                \
    })
#define INTERRUPT_RESTORE(_sreg) SREG = _sreg
