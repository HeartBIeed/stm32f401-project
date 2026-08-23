#include "main.h"
uint8_t hour;
uint8_t min;
uint8_t sec;

void gpio_init()
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; 
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; 

GPIOC->MODER |= ( 1 << (13*2)); //PC13 / output 01

}

void print_Time()
{

	RTC_getTime(&hour,&min,&sec);
	char text[20]={"\0"};	
	sprintf(text, "TIME %02d:%02d:%02d \r\n",hour,min,sec);
	usart1_ptr_str(text); 

}

int main(void)
{
	PLL_enable();
	SysTick_init();
	DWT_init();
	gpio_init();
	UART_init(9600);
	//SPI1_init();
	RTC_init();
//	RTC_setTime(4,41);
usart1_ptr_str("start 76757465 \r\n"); 

uint32_t start[3] = {0}; // нулевые стартовые значения 
						 // для неблокирующих задержек

 while(1){


if (ms_ticks - start[0] >= 1000) 
	{
		GPIOC->ODR ^= (1 << 13);
		start[0] = ms_ticks;
		print_Time();
	}


	}
}

