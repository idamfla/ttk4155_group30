/*
 * fonts.h
 *
 * Library of fonts
 * Large: 8x8, normal: 5x7 and small: 4x7
 */
#ifndef FONTS_H_
#define FONTS_H_

#include <avr/pgmspace.h>

#define FONTS_PRINTABLE_CHAR_START 32U
#define FONTS_CPY_CHAR(font, dest, c) \
    memcpy_P(dest, &font[(uint8_t)c - FONTS_PRINTABLE_CHAR_START][0], sizeof(font[0]));

// Font 8x8 - Large
extern const unsigned char font8[95][8];

// Font 5x7 - normal
extern const unsigned char font5[95][5];

// Font 4x6 - Small
extern const unsigned char font4[95][4];

#endif /* FONTS_H_ */
