#ifndef ST7796_H_
#define ST7796_H_

#include "spi.h"

void ST7796_init();
void ST7796_SetRegion(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

void DATA_mode();
void CMD_mode();

void ST7796_cmd(uint8_t cmd);
void ST7796_data(uint8_t data);

void CS_high();
void CS_low();

#endif /* ST7796_H_ */
