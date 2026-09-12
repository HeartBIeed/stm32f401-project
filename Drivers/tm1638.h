#ifndef TM1638_H_
#define TM1638_H_

#include "main.h"

void tm1638_init();
uint8_t tm1638_read();
void tm1638_set_digit(uint8_t digit,uint8_t position, uint8_t dot);
void tm1638_set_led(uint8_t state, uint8_t position);


#endif /* TM1638_H_ */