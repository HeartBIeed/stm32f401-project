#include "spi.h"
#include "ST7796.h"

//GPIOA
#define RST 2
#define DC 3
#define CS 4

void ST7796_setPins(){

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 

	GPIOA->MODER &= ~((3 << (RST*2))|(3 << (DC*2))|(3 << (CS*2))); 
	GPIOA->MODER |= ((1 << (RST*2))|(1 << (DC*2))|(1 << (CS*2))); // output

	GPIOA->OSPEEDR &= ~((3 << (RST*2))|(3 << (DC*2))|(3 << (CS*2))); 
	GPIOA->OSPEEDR |= ((3 << (RST*2))|(3 << (DC*2))|(3 << (CS*2))); //high

	GPIOA->BSRR |= (1 << RST); 

}

void DATA_mode(){
	GPIOA->BSRR |= (1 << DC); 
}

void CMD_mode(){
	GPIOA->BSRR |= (1 << (DC+16)); 
}

void CS_high(){
	GPIOA->BSRR |= (1 << CS); 
}

void CS_low(){
	GPIOA->BSRR |= (1 << (CS+16)); 
}

void ST7796_reset(){

	GPIOA->BSRR |= (1 << RST); 
		_delay_ms(10);	
	GPIOA->BSRR |= (1 << (RST+16)); 
		_delay_ms(10);	
	GPIOA->BSRR |= (1 << RST); 

}

void ST7796_cmd(uint8_t cmd){

	CMD_mode();
	SPI1_send_byte(cmd);
}

void ST7796_data(uint8_t data){

	DATA_mode();
	SPI1_send_byte(data);
}

void ST7796_init(){

ST7796_setPins();

ST7796_reset();
	_delay_ms(10);

CS_low();
		_delay_ms(10);

	ST7796_cmd(0x01);
		_delay_ms(120);
	
	ST7796_cmd(0x11);
		_delay_ms(120);

	ST7796_cmd(0xF0);
	ST7796_data(0xC3);

	ST7796_cmd(0xF0);
	ST7796_data(0x96);

	ST7796_cmd(0x36); // MADCTL — ориентация
	ST7796_data(0x48); // портрет
//	ST7796_data(0x28); // альбом

	ST7796_cmd(0x3A);  // COLMOD — формат пикселя
	ST7796_data(0x55); // 16 bit/pixel, RGB565

	ST7796_cmd(0xB4);
	ST7796_data(0x01);

	ST7796_cmd(0xB7);
	ST7796_data(0xC6);

	ST7796_cmd(0xC0);
	ST7796_data(0x80);
	ST7796_data(0x45);

	ST7796_cmd(0xC1);
	ST7796_data(0x13);

	ST7796_cmd(0xC2);
	ST7796_data(0xA7);

	ST7796_cmd(0xC5);
	ST7796_data(0x0A);

	ST7796_cmd(0xE8);
	ST7796_data(0x40);
	ST7796_data(0x8A);
	ST7796_data(0x00);
	ST7796_data(0x00);
	ST7796_data(0x29);
	ST7796_data(0x19);
	ST7796_data(0xA5);
	ST7796_data(0x33);

	ST7796_cmd(0xE0);
	ST7796_data(0xD0);
	ST7796_data(0x08);
	ST7796_data(0x0F);
	ST7796_data(0x06);
	ST7796_data(0x06);
	ST7796_data(0x33);
	ST7796_data(0x30);
	ST7796_data(0x33);
	ST7796_data(0x47);
	ST7796_data(0x17);
	ST7796_data(0x13);
	ST7796_data(0x13);
	ST7796_data(0x2B);
	ST7796_data(0x31);

	ST7796_cmd(0xE1);
	ST7796_data(0xD0);
	ST7796_data(0x0A);
	ST7796_data(0x11);
	ST7796_data(0x0B);
	ST7796_data(0x09);
	ST7796_data(0x07);
	ST7796_data(0x2F);
	ST7796_data(0x33);
	ST7796_data(0x47);
	ST7796_data(0x38);
	ST7796_data(0x15);
	ST7796_data(0x16);
	ST7796_data(0x2C);
	ST7796_data(0x32);

	//*** Disable command ***/
	ST7796_cmd(0xF0);
	ST7796_data(0x3C);

	ST7796_cmd(0xF0);
	ST7796_data(0x69);
	//***********************/

	ST7796_cmd(0x21); // Display Inversion ON
	ST7796_cmd(0x29); // Display ON

		_delay_ms(50);
}


void ST7796_SetRegion(uint16_t x, uint16_t y, uint16_t w, uint16_t h){

	ST7796_cmd(0x2A);
	ST7796_data(0);
	ST7796_data(x);
	ST7796_data(0);
	ST7796_data(x + w - 1);

	ST7796_cmd(0x2B);
	ST7796_data(0);
	ST7796_data(y);
	ST7796_data(0);
	ST7796_data(y + h - 1);

	ST7796_cmd(0x2C);

}



