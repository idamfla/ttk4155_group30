/**
 * @file spi_transfer.h
 * @author Marius Bär
 * @brief Typedef for SPI data transfer
 * @date 2025-09-25
 */

#pragma once
#ifndef SPI_TRANSFER_H
#    define SPI_TRANSFER_H

#    include <stdint.h>

/**
 * Data to transfer
 * @param tx_data Pointer to the tx_data
 * @param rx_data Pointer to the rx_data
 * @param length Length/size of data
 * @param slave_idx Which slave to send the data to
 */
typedef struct {
    const uint8_t* tx_data;
    volatile uint8_t* rx_data;
    uint8_t length;
    uint8_t slave_idx;
    void (*transfer_cmplt_cbk)(void* param);
    void* param;
} spi_transfer_t;

#endif /* SPI_TRANSFER_H */
