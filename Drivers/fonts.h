/*
 * fonts.h
 *
 *  Created on: 1 апр. 2020 г.
 *      Author: dima
 * https://github.com/stDstm/Example_STM32F103/blob/master/Display_TFT_ILI9341/Inc/fonts.h
 */

#ifndef FONTS_H_
#define FONTS_H_

#include "main.h"

typedef struct {
    const uint8_t width;
    uint8_t height;
    const uint16_t *data;
} FontDef;

extern FontDef Font_7x10;
extern FontDef Font_11x18;
extern FontDef Font_11x18_Cyrillic;
extern FontDef Font_16x26;



#endif /* FONTS_H_ */
