#include "st7735.h"

void ST7735_setPins(){

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 

	GPIOA->MODER &= ~((3 << (RST*2))|(3 << (AO*2))|(3 << (CS*2))); 
	GPIOA->MODER |= ((1 << (RST*2))|(1 << (AO*2))|(1 << (CS*2))); // output

	GPIOA->OSPEEDR &= ~((3 << (RST*2))|(3 << (AO*2))|(3 << (CS*2))); 
	GPIOA->OSPEEDR |= ((3 << (RST*2))|(3 << (AO*2))|(3 << (CS*2))); //high

	GPIOA->BSRR |= (1 << RST); 


}

void DATA_mode(){
	GPIOA->BSRR |= (1 << AO); 
}

void CMD_mode(){
	GPIOA->BSRR |= (1 << (AO+16)); 
}

void CS_high(){
	GPIOA->BSRR |= (1 << CS); 
}

void CS_low(){
	GPIOA->BSRR |= (1 << (CS+16)); 
}

void ST7735_reset(){

	GPIOA->BSRR |= (1 << RST); 
		_delay_ms(10);	
	GPIOA->BSRR |= (1 << (RST+16)); 
		_delay_ms(10);	
	GPIOA->BSRR |= (1 << RST); 

}

void ST7735_cmd(uint8_t cmd){

	CMD_mode();
	SPI1_sendByte(cmd);
}

void ST7735_data(uint8_t data){

	DATA_mode();
	SPI1_sendByte(data);
}

void ST7735_init(){

	ST7735_setPins();

	ST7735_reset();
		_delay_ms(10);
	CS_low();
		_delay_ms(10);

	ST7735_cmd(0x11);
		_delay_ms(120);

	ST7735_cmd(0x3A);
	ST7735_data(0x05);

	ST7735_cmd(0x36);
	ST7735_data(0x14);

	ST7735_cmd(0x29);
		_delay_ms(100);

}


int ST7735_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {

  if((x >= 128) || (y >= 160)) return 0;
  if(w <= 0 || h <= 0) return 0;

  if(x + w >  128)  w = 128  - x;
  if(y + h > 160) h = 160 - y;

CS_low();

	ST7735_cmd(0x2A);
	ST7735_data(0);
	ST7735_data(x);
	ST7735_data(0);
	ST7735_data(x+w-1);

	ST7735_cmd(0x2B);
	ST7735_data(0);
	ST7735_data(y);
	ST7735_data(0);
	ST7735_data(y+h-1);

	ST7735_cmd(0x2C);

	DATA_mode();

for(int16_t i=0; i<h; i++) {
    for(int16_t j=0; j<w; j++) {

	SPI1_sendByte(color >> 8);
	SPI1_sendByte(color & 0xFF);

    }
  }

 CS_high();
 return 1;
}


void ST7735_SetRegion(uint16_t x, uint16_t y, uint16_t w, uint16_t h){

	ST7735_cmd(0x2A);
	ST7735_data(0);
	ST7735_data(x);
	ST7735_data(0);
	ST7735_data(h);

	ST7735_cmd(0x2B);
	ST7735_data(0);
	ST7735_data(y);
	ST7735_data(0);
	ST7735_data(w);

	ST7735_cmd(0x2C);

}



int ST7735_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) return 0;
    
    // Отправка команды на установку области
    ST7735_SetRegion(x, y, x, y);

    	DATA_mode();
		SPI1_sendByte(color >> 8);
		SPI1_sendByte(color & 0xFF);
	return 1;
}


void ST7735_DrawChar(uint16_t x, uint16_t y, char c, uint16_t color) {

    const uint8_t* char_data = font8x8[(int)c];

    // Проходим по всем 8 строкам
    for (int row = 0; row < 8; row++) {
        uint8_t row_data = char_data[row];
            // Проходим по всем 8 стлбцам
		for (int col = 0; col < 8; col++){

			if (row_data & (0x80 >> col)){ //0x80 - 0b10000000
             // Рисуем пиксель, если бит = 1
             ST7735_DrawPixel(x + col, y + row, color);
			} //end if
		} //end for 2
	} //end for 1	
}

void ST7735_DrawString(uint16_t x, uint16_t y, const char* str, uint16_t color) {
CS_low();

    uint16_t cursor_x = x;

    while (*str) {

        ST7735_DrawChar(cursor_x, y, *str, color);
        cursor_x += 8;  // Ширина символа

        // Проверка выхода за границу экрана
        if (cursor_x + 8 > SCREEN_WIDTH) {
            cursor_x = x;
            y += 8;  // Переход на следующую строку
        }

     str++; 
     }  //end while
 CS_high(); 
}

