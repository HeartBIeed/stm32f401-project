#include "uart.h"

// PORT A 
#define U1RX 10
#define U1TX 9

static  ring_buffer uart1_rx_buffer; // создали буфер
static  uint8_t uart1_rx_data[256]; // массив под буфер

void USART1_init(uint16_t baudrate){

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

	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE | USART_CR1_UE; 

	ring_buffer_init(&uart1_rx_buffer, uart1_rx_data, sizeof(uart1_rx_data) );
//передали адрес структуры настройки буфера
// передали массив и его размер

NVIC_ClearPendingIRQ(USART1_IRQn);
NVIC_EnableIRQ(USART1_IRQn);
}

void USART1_send_byte(uint8_t tx_data){
	while ((USART1->SR & USART_SR_TXE) == 0U) {
	}
	USART1->DR = tx_data;
}

uint8_t USART1_recieve_byte(){

	uint32_t timeout = TIMEOUT;

		while ((USART1->SR & USART_SR_RXNE) == 0U)
		{
			if(--timeout == 0) break;
		}
		
		return (uint8_t)USART1->DR;
}

void USART1_send_string(char *str){
		while (*str) USART1_send_byte(*str++);
}





void USART1_IRQHandler() { // запись байта в ринг буфер

	if (USART1->SR & USART_SR_RXNE)
	{
		uint8_t byte = (uint8_t)USART1->DR;
		ring_buffer_put(&uart1_rx_buffer, byte);
	}
}	


int USART1_read_byte(uint8_t *byte) //получение байта
{
    return ring_buffer_get(&uart1_rx_buffer, byte);
}






