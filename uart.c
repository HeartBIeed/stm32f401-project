#include "uart.h"

volatile uint8_t uart1_flag = 0;
char data_buffer[20];
volatile uint8_t usart_data_buffer[32]; //буффер uart

void UART_init(uint16_t baud)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN; //APB bus

//moder 0b10 - alt 2 bit
	GPIOA->MODER &= ~( 0x3 << (U1TX*2)); // clear
	GPIOA->MODER |= ( 0x2 << (U1TX*2)); //set tx

	GPIOA->MODER &= ~( 0x3 << (U1RX*2)); //clear
	GPIOA->MODER |= ( 0x2 << (U1RX*2)); //set rx

//otyper 0- push-pull 1 bit
	GPIOA->OTYPER &= ~((1<<U1TX)|(1<<U1RX)); //usart pins to  0

//speed 10 high 2 bit
	GPIOA->OSPEEDR |= ((0x2<<(U1TX*2))|(0x2<<(U1RX*2))); //usart pin to  0b10

	GPIOA->AFR[1] &= ~((0xF<<((U1TX-8)*4))|(0xF<<((U1RX-8)*4))); //clear
	GPIOA->AFR[1] |= ((0x7<<((U1TX-8)*4))|(0x7<<((U1RX-8)*4)));  //0x7 alt func/ 4 bit on 1 pin

	USART1->CR1 = 0;
	USART1->CR2 = 0;
	USART1->CR3 = 0;

	uint16_t div_baud8 = baud; // /8
	USART1->BRR = (FCPU+(div_baud8/2))/div_baud8; 

	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE; 
	//TX RX usart enable

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

void usart1_rxen_flag()
	{

	if (USART1->SR & USART_SR_RXNE)
		{
		data_buffer[0] = (uint8_t)USART1->DR;
		uart1_flag = 1;
		}

		else
		{
		uart1_flag =0;
		}
		
	}


void usart1_echo()
	{
		if (USART1->SR & USART_SR_RXNE) // поправить под флаг
		{
		usart1_send_byte((uint8_t)USART1->DR);
	
		}
	
	}

void usart1_ptr_str(char *str) // TX string
	{
		while (*str) usart1_send_byte(*str++);
	}


//************************************** */


int USART_commands(){

	if (strncmp((char*)usart_data_buffer,"st",2) == 0) 
	{

		char *command = strtok((char*)usart_data_buffer," ");
		char *h_char = strtok(NULL, ",");
		char *m_char = strtok(NULL, ",");

		int h = atoi(h_char);
		int m = atoi(m_char);

		RTC_setTime(h,m);


		char string[32];
		sprintf(string, "SET TIME -> %2d:%2d \r\n",h,m);
		usart1_ptr_str(string);
	 	usart_data_buffer[0] = '\0';

	 		return 1;

	} else {
			return 0;
	}
		
}






