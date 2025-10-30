/**
 * @file usart.h
 * @author Marius Bär
 * @brief Header file for usart.c
 * @date 2025-09-08
 */

#ifndef USART_H
#define USART_H

#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

#define USART0 ((usart_t *)(&UBRR0L))
#define USART1 ((usart_t *)(&UBRR1L))

typedef struct {
    volatile uint8_t ubrrl;
    volatile uint8_t ucsrb;
    volatile uint8_t ucsra;
    volatile uint8_t udr;
} usart_t;

typedef void (*usart_rxcbk_t)(uint8_t data);

void usart_init(volatile usart_t *usart, uint16_t ubrr);
void usart_init_interrupt(volatile usart_t *usart, uint16_t ubrr, usart_rxcbk_t rx_callback);
bool usart_tx_ready(volatile usart_t *usart);
void usart_transmit(volatile usart_t *usart, uint8_t data);

#endif /* USART_H */
