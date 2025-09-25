/**
 * @file spi.c
 * @author Thomas Berg
 * @author Ida Flakstad
 * @brief brief
 * @date 2025-09-23
 */

#include "spi.h"

#include <avr/interrupt.h>
#include <string.h>

SPITransferData transfer;
volatile uint16_t index;

void spi_master_init(void) {
    /* Set MOSI and SCK output, all others input */
    DDR_SPI = (1 << DD_MOSI) | (1 << DD_SCK);
    // DDR_SPI &= ~(1 << PB6);  // MISO as input
    /* Enable SPI, Master, set clock rate fck/16, interrupt*/
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0) | (1 << SPIE);

    /* Set slave pins as output, and set them high (SS\bar) */
    DDR_SLAVES |= (1 << SS1) | (1 << SS2);
    PORT_SLAVES |= (1 << SS1) | (1 << SS2);
}

void spi_start_data_transfer(const SPITransferData *t) {
    index = 0;
    memcpy(&transfer, t, sizeof(transfer));
    PORT_SLAVES &= ~(1 << transfer.ss);
    SPDR = transfer.data[index];
}

// /* Reading from slaves */
// uint8_t spi_master_transmit(uint8_t slaveSS, uint8_t data) {
//     PORT_SLAVES &= ~(1 << slaveSS);
//     SPDR = data;
//     while (!(SPSR & (1 << SPIF)));

//     uint8_t recieved = SPDR;
//     PORT_SLAVES |= (1 << slaveSS);

//     return recieved;
// }

// void spi_write_word(uint8_t slaveSS, char *str, uint8_t n) {
//     for (uint8_t i = 0; i < n; i++) {
//         spi_master_transmit(slaveSS, str[i]);
//     }
// }

ISR(SPI_STC_vect) {
    if (transfer.recieved) {
        transfer.recieved[index] = SPDR;
    }
    PORT_SLAVES |= (1 << transfer.ss);

    index++;
    if (index < transfer.length) {
        PORT_SLAVES &= ~(1 << transfer.ss);
        SPDR = transfer.data[index];
    }
}