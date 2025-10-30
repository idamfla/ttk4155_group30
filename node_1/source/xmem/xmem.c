/**
 * @file xmem.c
 * @author Marius Bär
 * @brief Initialize external memory
 * @date 2025-09-18
 */

#include "xmem.h"

#include <avr/io.h>

/**
 * @brief Initialize external memory with no wait-states and 12 address lines.
 */
void xmem_init(void) {
    // Configure external memory high mask to only use four lines of the high byte of the address.
    // This release the pins PC4 to PC7 (for JTAG).
    SFIOR = (1 << XMM2);
    // Enable external SRAM with no wait-states (see table 3 in the datasheet).
    MCUCR = (1 << SRE);
}
