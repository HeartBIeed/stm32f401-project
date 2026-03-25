#ifndef SPI_H_
#define SPI_H_

#include "main.h"

#define PA4 4
#define PA5 5
#define PA6 6
#define PA7 7

void SPI1_init();
uint8_t SPI1_sendByte(uint8_t tx_data);
void ST7735_test();

#endif /* SPI_H_ */
