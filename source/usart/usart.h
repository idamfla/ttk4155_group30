/**
 * @file usart.h
 * @author Marius Bär
 * @brief Header file for usart.c
 * @date 2025-09-08
 */

#ifndef USART_H
#define USART_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    volatile uint8_t ubrrl;
    volatile uint8_t ucsrb;
    volatile uint8_t ucsra;
    volatile uint8_t udr;
} usart_t;

void usart_init(volatile usart_t *usart, uint16_t ubrr);
bool usart_tx_ready(volatile usart_t *usart);
void usart_transmit(volatile usart_t *usart, uint8_t data);

#endif /* USART_H */
