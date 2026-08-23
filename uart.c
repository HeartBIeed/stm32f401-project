#include "uart.h"

// PORT A 
#define U1RX 10
#define U1TX 9

void USART1_init(uint16_t baudrate)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN; //APB bus

	MODIFY_REG(GPIOA->MODER, (3U << (U1TX*2)), (2U << (U1TX*2)));
	MODIFY_REG(GPIOA->MODER, (3U << (U1RX*2)), (2U << (U1RX*2)));

	CLEAR_BIT(GPIOA->OTYPER, (1U << U1TX) | (1U << U1RX));

	MODIFY_REG(GPIOA->OSPEEDR, (3U << (U1TX*2)), (2U << (U1TX*2)));
	MODIFY_REG(GPIOA->OSPEEDR, (3U << (U1RX*2)), (2U << (U1RX*2)));

	MODIFY_REG(GPIOA->AFR[1], (0xFU << ((U1TX - 8U) * 4U)),
							 (7U << ((U1TX - 8U) * 4U)));
	MODIFY_REG(GPIOA->AFR[1], (0xFU << ((U1RX - 8U) * 4U)),
							 (7U << ((U1RX - 8U) * 4U)));


	USART1->CR1 = 0;
	USART1->CR2 = 0;
	USART1->CR3 = 0;

	USART1->BRR = (FCPU + (baudrate / 2U)) / baudrate;

	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE; 
}

void USART1_send_byte(uint8_t tx_data)
{
	while ((USART1->SR & USART_SR_TXE) == 0U) {
	}
	USART1->DR = tx_data;
}

uint8_t USART1_recieve_byte()
{

	uint32_t timeout = TIMEOUT;

		while ((USART1->SR & USART_SR_RXNE) == 0U)
		{
			if(--timeout == 0) break;
		}
		
		return (uint8_t)USART1->DR;
}


void USART1_send_string(char *str)
{
		while (*str) USART1_send_byte(*str++);
}


void USART1_IRQHandler() 
	{

	if (USART1->ISR & USART_ISR_RXNE) 

		{
		char data_rx = USART1->RDR;
		usart1_flag =1; // поставим флаг

		if (data_rx == '\r' || usart_index_buffer >= sizeof(usart_data_buffer)-1) //если конец строки или переполнение
			{
				usart_data_buffer[usart_index_buffer] = '\0'; //вставляем 0-терминатор
				usart_index_buffer = 0;
			}		
				
		else	
			{
				usart_data_buffer[usart_index_buffer] = data_rx;
				usart_index_buffer++;
			}
			
		}


	}	









