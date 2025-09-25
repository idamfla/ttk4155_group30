/**
 * @file spi_transfer.h
 * @author Marius Bär
 * @brief Typedef for SPI data transfer
 * @date 2025-09-25
 */

#ifndef SPI_TRANSFER_H
#define SPI_TRANSFER_H

#include <stdint.h>

typedef struct {
    const uint8_t *tx_data;
    volatile uint8_t *rx_data;
    uint8_t length;
    uint8_t slave_idx;
} spi_transfer_t;

#endif /* SPI_TRANSFER_H */
