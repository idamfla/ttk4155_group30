/**
 * @file usart.h
 * @author Marius Bär
 * @brief Header file for usart.c
 * @date 2025-09-08
 */

#ifndef USART_H
#define USART_H

#include <stdint.h>

void usart0_init(uint16_t ubrr);
void usart1_init(uint16_t ubrr);

#endif /* USART_H */
