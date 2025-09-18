/**
 * @file gpio.h
 * @author Marius Bär
 * @brief Helper functions/macros for GPIO manipulation
 * @date 2025-09-18
 */

#ifndef GPIO_H
#define GPIO_H

#define SET_PIN(port, pin)    ((port) |= ((uint8_t)1U << (pin)))
#define CLEAR_PIN(port, pin)  ((port) &= ~((uint8_t)1U << (pin)))
#define TOGGLE_PIN(port, pin) ((port) ^= ((uint8_t)1U << (pin)))
#define READ_PIN(port, pin)   (((port) >> (pin)) & (uint8_t)1U)

#endif /* GPIO_H */
