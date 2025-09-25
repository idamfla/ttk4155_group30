/**
 * @file spi.h
 * @author Thomas Berg
 * @author Ida Flakstad
 * @brief brief
 * @date 2025-09-23
 */

#pragma once;
#include <avr/io.h>

#define DDR_SPI DDRB
#define DD_MOSI DDB5
#define DD_SCK  DDB7
// #define SS DDB4
#define DDR_SLAVES  DDRA
#define PORT_SLAVES PORTA
#define SS1         DDA0
#define SS2         DDA1
#define SS3         DDA2
#define BUFFER_DATA 0x00

void SPI_MasterInit(void);
void SPI_MasterTransmit(char cData);

void SPI_write(uint8_t data);
void SPI_write_to_slave(uint8_t slaveSS, uint8_t data);

uint8_t SPI_read();
uint8_t SPI_read_from_slave(uint8_t slaveSS);

// void SPI_SlaveInit(void);
// char SPI_SlaveReceive(void);