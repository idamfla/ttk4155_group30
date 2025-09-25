/**
 * @file spi.c
 * @author Thomas Berg
 * @author Ida Flakstad
 * @brief brief
 * @date 2025-09-23
 */

#include "spi.h"

void SPI_MasterInit(void) {
    /* Set MOSI and SCK output, all others input */
    DDR_SPI = (1 << DD_MOSI) | (1 << DD_SCK);
    // DDR_SPI &= ~(1 << PB6);  // MISO as input
    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);

    /* Set slave pins as output, and set them high (SS\bar) */
    DDR_SLAVES = (1 << SS1) | (1 << SS2) | (1 << SS2);
    PORT_SLAVES |= (1 << SS1) | (1 << SS2) | (1 << SS2);
}
void SPI_MasterTransmit(char cData) {
    /* Start transmission */
    SPDR = cData;
    /* Wait for transmission complete */
    while (!(SPSR & (1 << SPIF)));
}



/* Writing to slaves */
void SPI_write(uint8_t data) {
    SPI_write_to_slave(SS1, data);
}

void SPI_write_to_slave(uint8_t slaveSS, uint8_t data) {
    PORT_SLAVES &= ~(1 << slaveSS);

    SPDR = data;
    while (!(SPDR & (1 << SPIF)));

    PORT_SLAVES |= (1 << slaveSS);
}



/* Reading from slaves */
uint8_t SPI_read() {
    return SPI_read_from_slave(SS1);
}
uint8_t SPI_read_from_slave(uint8_t slaveSS) {
    PORT_SLAVES &= ~(1 << slaveSS);
    SPDR = BUFFER_DATA;
    while (!(SPDR & (1 << SPIF)));

    uint8_t recieved = SPDR;
    PORT_SLAVES |= (1 << slaveSS);

    return recieved;
}

// void SPI_SlaveInit(void) {
//     /* Set MISO output, all others input */
//     DDR_SPI = (1 << DD_MISO);
//     /* Enable SPI */
//     SPCR = (1 << SPE);
// }
// char SPI_SlaveReceive(void) {
//     /* Wait for reception complete */
//     while (!(SPSR & (1 << SPIF)));
//     /* Return data register */
//     return SPDR;
// }