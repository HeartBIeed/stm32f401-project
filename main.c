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

void RTC_print_time()
{

	RTC_getTime(&hour,&min,&sec);
	char text[20]={"\0"};	
	sprintf(text, "TIME %02d:%02d:%02d \r\n",hour,min,sec);
	USART1_send_string(text); 

}

int main(void)
{
	SystemClock_PLL_84MHz();
	SysTick_init();
	DWT_init();
	gpio_init();
	USART1_init(9600);
	//SPI1_init();
	RTC_init();
//	RTC_setTime(4,41);
зUSART1_send_string("start \r\n"); 

uint32_t start[3] = {0}; // нулевые стартовые значения 
						 // для неблокирующих задержек

 while(1){


if (ms_ticks - start[0] >= 1000) 
	{
		GPIOC->ODR ^= (1 << 13);
		start[0] = ms_ticks;
		RTC_print_time();
	}


	}
}

