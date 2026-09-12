#include "spi.h"
#include "main.h"

#define SCL 5 // PA5
#define SDA 7 // PA7

void SPI1_init(){

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; 

	GPIOA->MODER &= ~(3 << (SCL*2))|(3 << (SDA*2)); 
	GPIOA->MODER |= (2 << (SCL*2))|(2 << (SDA*2));

	GPIOA->OTYPER &= ~((1 << SCL)|(1 << SDA));
	GPIOA->OSPEEDR |= ((2 << (SCL*2))|(2 << (SDA*2))); 

	GPIOA->AFR[0] &= ~((0xF << (SCL*4))|(0xF << (SDA*4)));  
	GPIOA->AFR[0] |= ((5 << (SCL*4))|(5 << (SDA*4))); //AF5

	SPI1->CR1 = 0;
	SPI1->CR2 = 0;

	SPI1->CR1 |= (1<<2); // Master
	SPI1->CR1 |= (1<<9); // SSM Software slave management
	SPI1->CR1 |= (1<<8); // SSI Internal slave select
	SPI1->CR1 |= (1<<3); // baud rate f/4

	SPI1->CR2 |= (7<<8); // Data size 8 bit

	SPI1->CR1 |= (1<<6); // SPI EN

}

uint8_t SPI1_send_byte(uint8_t tx_data){

	while (!(SPI1->SR & SPI_SR_TXE)); 
	*(volatile uint8_t*)&SPI1->DR = tx_data; 
//	while (!(SPI1->SR & SPI_SR_RXNE)); 
	while(SPI1->SR & SPI_SR_BSY);
	return *(volatile uint8_t*)&SPI1->DR;
}


