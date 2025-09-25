/**
 * @file spi_queue.c
 * @author Marius Bär
 * @brief Queue to hold SPI transfers
 * @date 2025-09-25
 */

#include "spi_queue.h"

bool spi_queue_push(spi_queue_t *queue, const spi_transfer_t *transfer) {
    if (queue->size >= queue->max_size) {
        return false;
    }
    ++queue->size;
    // Fill the queue backwards
    *queue->back = *transfer;
    if (queue->back > queue->buffer) {
        --queue->back;
    } else {
        queue->back = queue->buffer + queue->max_size - 1;
    }
    return true;
}

spi_transfer_t spi_queue_pop(spi_queue_t *queue) {
    spi_transfer_t front = *(queue->front);
    if (queue->front > queue->buffer) {
        --queue->front;
    } else {
        queue->front = queue->buffer + queue->max_size - 1;
    }
    --queue->size;
    return front;
}
