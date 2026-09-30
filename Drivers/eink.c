#include "spi.h"
#include "eink.h"

//GPIOA
#define BUSY 1
#define RESET 2
#define DC 3
#define CS 4
// SPI SCL 5
// SPI SDA 7

void EINK_setPins(){

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 

	CLEAR_BIT(GPIOA->MODER, (3 << (BUSY*2)));
	MODIFY_REG(GPIOA->MODER, (3 << (RESET*2)), (1 << (RESET*2)));
	MODIFY_REG(GPIOA->MODER, (3 << (DC*2)), (1 << (DC*2)));
	MODIFY_REG(GPIOA->MODER, (3 << (CS*2)), (1 << (CS*2)));

	MODIFY_REG(GPIOA->OSPEEDR, (3 << (BUSY*2)), (3 << (BUSY*2)));
	MODIFY_REG(GPIOA->OSPEEDR, (3 << (RESET*2)), (3 << (RESET*2)));
	MODIFY_REG(GPIOA->OSPEEDR, (3 << (DC*2)), (3 << (DC*2)));
	MODIFY_REG(GPIOA->OSPEEDR, (3 << (CS*2)), (3 << (CS*2)));


	GPIOA->BSRR |= (1 << RESET);
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

void EINK_reset(){

	GPIOA->BSRR |= (1 << RESET); 
		_delay_ms(10);	
	GPIOA->BSRR |= (1 << (RESET+16)); 
		_delay_ms(10);	
	GPIOA->BSRR |= (1 << RESET); 
}

void EINK_cmd(uint8_t cmd){

	CMD_mode();
	SPI1_send_byte(cmd);
}

void EINK_data(uint8_t data){

	DATA_mode();
	SPI1_send_byte(data);
}

uint8_t wait_BUSY(){

	int timeout = 1000000;

	while(!(READ_BIT(GPIOA->IDR,(1 << BUSY)))){
		if (--timeout == 0) return 0;
	}

	return 1;
}


void EINK_init(){

	EINK_setPins();

	EINK_reset();
		_delay_ms(50);

	CS_low();
		_delay_ms(100);

	EINK_cmd(0x4D);
	EINK_data(0x78);

	EINK_cmd(0x00);
	EINK_data(0x0F);
	EINK_data(0x29);

	EINK_cmd(0x06);
	EINK_data(0x0D);
	EINK_data(0x12);
	EINK_data(0x30);
	EINK_data(0x20);
	EINK_data(0x19);
	EINK_data(0x2A);
	EINK_data(0x22);


	EINK_cmd(0x30);
	EINK_data(0x08);

	EINK_cmd(0x50);
	EINK_data(0x37);

	EINK_cmd(0x61); // разрешение
	EINK_data(0x00); // X 0
	EINK_data(0xC8); // 200 

	EINK_data(0x00); // Y 0
	EINK_data(0xC8); // 200

	EINK_cmd(0xE9);
	EINK_data(0x01);

		_delay_ms(100);
		
	CS_high();
		_delay_ms(100);

}


void EINK_draw(uint8_t *data){

	CS_low();

	EINK_cmd(0x04); // pwr on
	_delay_ms(1000);//!!!!!!!!!!!!!!!!

	if (!wait_BUSY()){

		CS_high();
		return;
	}

	EINK_cmd(0x10); // start tx

	for (uint16_t i = 0; i < 10000; i++)
	{
//			EINK_data(data[i]);
//			EINK_data(0xFF); // RED test
			EINK_data(0xAA); // YELLOW test
//			EINK_data(0x55); // WHITE test
//			EINK_data(0x00); // BLACK test

	}


	EINK_cmd(0x12); // показать
	EINK_data(0x00);

	_delay_ms(1000);//!!!!!!!!!!!!!!!!

	if (!wait_BUSY()){

		CS_high();
		return;
	}

	EINK_cmd(0x02); //pwr off
	EINK_data(0x00);
/*
	EINK_cmd(0x07); //sleep
	EINK_data(0xA5);
*/
	CS_high();

}



