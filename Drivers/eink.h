#ifndef EINK_H_
#define EINK_H_

#include "spi.h"

typedef enum {
    EINK_FILL_BLACK  = 0x00,
    EINK_FILL_WHITE  = 0x55,
    EINK_FILL_YELLOW = 0xAA,
    EINK_FILL_RED    = 0xFF
} EINK_fill_Color;

void EINK_init();
void EINK_draw(uint8_t *data);
void EINK_fill(EINK_fill_Color color);

#endif /* EINK_H_ */
