#include "main.h"


void gpio_init()
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; 
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; 

//	GPIOA->MODER |= ( 0x01 << (5*2)); //pa5 / output 0x01
//	GPIOA->MODER |= ( 0x01 << (6*2)); //6
//	GPIOA->MODER |= ( 0x01 << (7*2)); //7

GPIOC->MODER |= ( 0x01 << (13*2)); //PC13 / output 01

//	GPIOB->MODER &= ~( 0xFF << (1*2)); //pb1 / input 0x00
//	GPIOB->PUPDR |= ( 0x01 << (1*2)); //pb1 / up 01

//	GPIOB->MODER |= ( 0x01 << (3*2)); //pb3 / output 01
}



int main(void)
{
//	for (volatile int i=0; i<100000; i++);

	PLL_enable();
//SystemClock_HSI_8MHz();

	SysTick_init();
	DWT_init();

	UART_init(9600);
	gpio_init();

	CLEAR_BIT(GPIOC->ODR, (1 << 13));

uint32_t start[3] = {0}; // нулевые стартовые значения 
						 // для неблокирующих задержек

 while( 1 )
	{
	usart1_rxen_flag();
	get_usart_command();


/*
	usart1_ptr_str("\033[0;31m RED TEXT \r\n\r\n"); 
	_delay_ms(10);

	usart1_ptr_str("\033[1;33m YELLOW \r\n\r\n"); 
	_delay_ms(10);

	usart1_ptr_str("\033[0;32m GREEN \033[0m\r\n\r\n");
		_delay_ms(1000);

*/




if (ms_ticks - start[0] >= 1000) 
	{
		GPIOC->ODR ^= (1 << 13);
		start[0] = ms_ticks;
	}


	}
}

