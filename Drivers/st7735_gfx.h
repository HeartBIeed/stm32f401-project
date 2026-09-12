#ifndef ST7735GFX_H_
#define ST7735GFX_H_

#include "main.h"
#include "st7735.h"
#include "fonts.h"

int ST7735_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, ST7735_Color color);
void ST7735_DrawChar(uint16_t x, uint16_t y, char c, ST7735_Color color, FontDef font);
void ST7735_DrawString(uint16_t x, uint16_t y, const char* str, ST7735_Color color, FontDef font);
void ST7735_DrawLine(int16_t x0, int16_t y0,int16_t x1, int16_t y1,ST7735_Color color);
void bresenhamCircle(int16_t x0, int16_t y0, int16_t radius, ST7735_Color color);
void bresenhamCircleFill(int16_t x0, int16_t y0, int16_t radius, ST7735_Color color);

#endif /* ST7735GFX_H_ */
