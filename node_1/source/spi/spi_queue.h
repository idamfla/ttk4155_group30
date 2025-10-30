/**
 * @file spi_queue.h
 * @author Marius Bär
 * @brief Header file for spi_queue.c
 * @date 2025-09-25
 */

#ifndef SPI_QUEUE_H
#define SPI_QUEUE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "spi_transfer.h"

typedef struct {
    size_t max_size;
    volatile size_t size;
    volatile spi_transfer_t *buffer;
    volatile spi_transfer_t *volatile back;
    volatile spi_transfer_t *volatile front;
} spi_queue_t;

bool spi_queue_push(spi_queue_t *queue, const spi_transfer_t *transfer);
spi_transfer_t spi_queue_pop(spi_queue_t *queue);

#endif /* SPI_QUEUE_H */
