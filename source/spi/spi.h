/**
 * @file spi.h
 * @author Thomas Berg
 * @author Ida Flakstad
 * @brief brief
 * @date 2025-09-23
 */

#pragma once
#include <avr/io.h>

#define DDR_SPI DDRB
#define DD_MOSI DDB5
#define DD_SCK  DDB7
// #define SS DDB4
#define DDR_SLAVES  DDRB
#define PORT_SLAVES PORTB
#define SS1         DDB3
#define SS2         DDB4
#define DUMMY_DATA  0x00

typedef struct {
    uint8_t *data;
    uint16_t length;
    uint8_t ss;
    uint8_t *recieved;
} SPITransferData;

void spi_master_init(void);
void spi_start_data_transfer(const SPITransferData *transfer);

// uint8_t spi_master_transmit(uint8_t slaveSS, uint8_t data);

// void spi_write_word(uint8_t slaveSS, char *str, uint8_t n);