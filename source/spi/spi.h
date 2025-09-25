/**
 * @file spi.h
 * @author Thomas Berg
 * @author Ida Flakstad
 * @brief brief
 * @date 2025-09-23
 */

#pragma once
#include <avr/io.h>

#include "spi_transfer.h"

typedef enum {
    spi_slave_disp_d,
    spi_slave_disp_c,
    spi_slave_io,
    spi_slave_can,
} spi_slave_t;

#define DDR_SPI DDRB
#define DD_MOSI DDB5
#define DD_SCK  DDB7
// #define SS DDB4
#define DDR_SLAVES  DDRB
#define PORT_SLAVES PORTB
#define SS1         DDB3
#define SS2         DDB4
#define DUMMY_DATA  0x00

void spi_master_init(void);
void spi_transfer(const spi_transfer_t *transfer);
