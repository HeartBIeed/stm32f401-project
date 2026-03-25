#include "spi.h"


void SPI1_init(){

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; 

	GPIOA->MODER &= ~((3 << (PA4*2))|(3 << (PA5*2))|(3 << (PA6*2))|(3 << (PA7*2))); 
	GPIOA->MODER |= ((2 << (PA4*2))|(2 << (PA5*2))|(2 << (PA6*2))|(2 << (PA7*2)));

	GPIOA->OTYPER &= ~((1 << PA4)|(1 << PA5)|(1 << PA6)|(1 << PA7));
	GPIOA->OSPEEDR |= ((2 << (PA4*2))|(2 << (PA5*2))|(2 << (PA6*2))|(2 << (PA7*2))); //pa9/10 to  0b10

	GPIOA->AFR[0] &= ~((0xF << (PA4*4))|(0xF << (PA5*4))|(0xF << (PA6*4))|(0xF << (PA7*4)));  
	GPIOA->AFR[0] |= ((5 << (PA4*4))|(5 << (PA5*4))|(5 << (PA6*4))|(5 << (PA7*4))); //AF5

	SPI1->CR1 = 0;
	SPI1->CR2 = 0;

	SPI1->CR1 |= (1<<2); // Master
	SPI1->CR1 |= (1<<9); // SSM Software slave management
	SPI1->CR1 |= (1<<8); // SSI Internal slave select
	SPI1->CR1 |= (1<<3); // baud rate f/4

	SPI1->CR2 |= (7<<8); // Data size 8 bit

	SPI1->CR1 |= (1<<6); // SPI EN

}

uint8_t SPI1_sendByte(uint8_t tx_data){

	while (!(SPI1->SR & SPI_SR_TXE)); 
	*(volatile uint8_t*)&SPI1->DR = tx_data; 
//	while (!(SPI1->SR & SPI_SR_RXNE)); 
	while(SPI1->SR & SPI_SR_BSY);
	return *(volatile uint8_t*)&SPI1->DR;
}


