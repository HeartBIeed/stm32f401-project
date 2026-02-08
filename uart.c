#include "uart.h"

volatile uint8_t uart1_flag = 0;
char data_buffer[20];

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

	uint16_t div_baud8 = baud/8;
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

void get_usart_command()
{
	if (uart1_flag)
		{
			if (strncmp((char*)data_buffer,"gamma",5) == 0) // сравниваем первые символы
			{
				usart1_ptr_str("gamma");
				uart1_flag = 0;
			}

			if (strncmp((char*)data_buffer,"time",4) == 0) 
			{
				usart1_ptr_str("time");
				uart1_flag = 0;
			}

			if (strncmp((char*)data_buffer,"snd",3) == 0) 
			{
			char *command = strtok(data_buffer," ");
			char *sound_mode = strtok(NULL, ",");

			if (strcmp(sound_mode, "on"))
				{
					usart1_ptr_str("Sound ON\r\n");
				}
			else if (strcmp(sound_mode, "off"))
				{
					usart1_ptr_str("Sound OFF\r\n");
				}

			uart1_flag = 0;

			}

		else
		{
			usart1_ptr_str("GET: ");
			usart1_ptr_str((char*)data_buffer);
			usart1_ptr_str("\r\n");
		}				

		uart1_flag = 0;

	}		

}









