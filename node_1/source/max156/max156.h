/**
 * @file max156.h
 * @author Marius Bär
 * @brief Header file for max156.c
 * @date 2025-09-18
 */

#ifndef MAX156_H
#define MAX156_H

#include <stdint.h>

typedef struct {
    uint8_t ch0;
    uint8_t ch1;
    uint8_t ch2;
    uint8_t ch3;
} max156_data_t;

void max156_init(void);
void max156_read(max156_data_t *data);
void max156_trigger_conversion(void);

#endif /* MAX156_H */
