/**
 * @file spi.h
 * @author Thomas Berg
 * @author Ida Flakstad
 * @brief brief
 * @date 2025-09-23
 */

#pragma once;
#include <avr/io.h>

#define MISO PB5;
#define MOSI ;

void SPI_MasterInit(void);
void SPI_MasterTransmit(char cData);

void SPI_SlaveInit(void);
char SPI_SlaveReceive(void);