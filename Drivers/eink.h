#ifndef EINK_H_
#define EINK_H_

#include "spi.h"

void DATA_mode();
void CMD_mode();

void EINK_cmd(uint8_t cmd);
void EINK_data(uint8_t data);

void CS_high();
void CS_low();

void EINK_init();
void EINK_draw(uint8_t *data);

#endif /* EINK_H_ */
