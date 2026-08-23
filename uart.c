#include "uart.h"


void UART_init(uint16_t baudrate)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN; //APB bus

	MODIFY_REG(GPIOA->MODER, (3U << (U1TX*2)), (2U << (U1TX*2)));
	MODIFY_REG(GPIOA->MODER, (3U << (U1RX*2)), (2U << (U1RX*2)));

	MODIFY_REG(GPIOA->OTYPER, (1<<U1TX), (1<<U1TX));
	MODIFY_REG(GPIOA->OTYPER, (1<<U1RX), (1<<U1RX));

	MODIFY_REG(GPIOA->OSPEEDR, (3U << (U1TX*2)), (2U << (U1TX*2)));
	MODIFY_REG(GPIOA->OSPEEDR, (3U << (U1RX*2)), (2U << (U1RX*2)));

	MODIFY_REG(GPIOA->OSPEEDR, (3U << (U1TX*2)), (2U << (U1TX*2)));
	MODIFY_REG(GPIOA->OSPEEDR, (3U << (U1RX*2)), (2U << (U1RX*2)));

	MODIFY_REG(GPIOA->AFR[1], (0xFU << ((U1TX - 8U) * 4U)),
							 (7U << ((U1TX - 8U) * 4U)));
	MODIFY_REG(GPIOA->AFR[1], (0xFU << ((U1RX - 8U) * 4U)),
							 (7U << ((U1RX - 8U) * 4U)));


	USART1->CR1 = 0;
	USART1->CR2 = 0;
	USART1->CR3 = 0;

	USART1->BRR = (FCPU+(baudrate))/baudrate; 

	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE; 
}

void usart1_send_byte(uint8_t tx_data)
{
		while ((USART1->SR & USART_SR_TXE) == 0);
	 	USART1->DR = tx_data;
}

uint8_t usart1_recieve_byte()
	{
		while ((USART1->SR & USART_SR_RXNE) == 0)
		{

		}
		
		return (uint8_t)USART1->DR;

	}


void usart1_ptr_str(char *str) // TX string
	{
		while (*str) usart1_send_byte(*str++);
	}


//************************************** */









