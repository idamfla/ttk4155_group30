/**
 * @file spi.h
 * @author Thomas Berg
 * @author Ida Flakstad
 * @brief brief
 * @date 2025-09-23
 */

#pragma once
#include <avr/io.h>
#include <stdbool.h>

#include "spi_transfer.h"

typedef enum {
    spi_slave_disp_d,
    spi_slave_disp_c,
    spi_slave_io,
    spi_slave_can,
} spi_slave_t;



void _spi_next_transfer(void);
void spi_master_init(void);
bool spi_transfer(const spi_transfer_t* transfer);
