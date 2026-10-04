#ifndef ST7796GFX_H_
#define ST7796GFX_H_

#include "main.h"
#include "ST7796.h"
#include "fonts.h"

int ST7796_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, ST7796_Color color);
void ST7796_DrawString(uint16_t x, uint16_t y, const char* str, ST7796_Color color, FontDef font);
void ST7796_DrawLine(int16_t x0, int16_t y0,int16_t x1, int16_t y1,ST7796_Color color);
void bresenhamCircle(int16_t x0, int16_t y0, int16_t radius, ST7796_Color color);
void bresenhamCircleFill(int16_t x0, int16_t y0, int16_t radius, ST7796_Color color);

#endif /* ST7796GFX_H_ */
