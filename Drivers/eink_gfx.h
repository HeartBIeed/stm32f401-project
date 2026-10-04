#ifndef EINKGFX_H_
#define EINKGFX_H_

#include "main.h"
#include "eink.h"
#include "fonts.h"

typedef enum {
    EINK_BLACK  = 0b00,
    EINK_WHITE  = 0b01,
    EINK_YELLOW = 0b10,
    EINK_RED    = 0b11
} EINK_Color;

int FB_FillRect(uint8_t *fbuffer,uint16_t x, uint16_t y, uint16_t w, uint16_t h, EINK_Color color);
void FB_DrawString(uint8_t *fbuffer,uint16_t x, uint16_t y, const char* str, EINK_Color color,FontDef font);
void FB_DrawLine(uint8_t *fbuffer,int16_t x0, int16_t y0,int16_t x1, int16_t y1,EINK_Color color);
void bresenhamCircle(uint8_t *fbuffer,int16_t x0, int16_t y0, int16_t radius, EINK_Color color);
void bresenhamCircleFill(uint8_t *fbuffer,int16_t x0, int16_t y0, int16_t radius, EINK_Color color);

#endif /* EINKGFX_H_ */
