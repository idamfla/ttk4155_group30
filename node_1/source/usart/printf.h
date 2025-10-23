/**
 * @file printf.h
 * @author Marius Bär
 * @brief Header file for printf.c
 * @date 2025-09-11
 */

#ifndef PRINTF_H
#define PRINTF_H

#include "usart.h"

void printf_init(volatile usart_t *usart, uint16_t ubrr);

#endif /* PRINTF_H */
