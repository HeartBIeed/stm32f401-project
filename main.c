#include "main.h"
uint8_t hour;
uint8_t min;
uint8_t sec;

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

void print_Time(){

	RTC_getTime(&hour,&min,&sec);
	char text[20]={"\0"};	
	sprintf(text, "TIME %02d:%02d:%02d \r\n",hour,min,sec);
	ST7735_DrawString(3,72, text, 0xFFFF);
	
	}

int main(void)
{

//	PLL_enable();
//SystemClock_HSE();
SystemClock_HSI_8MHz();
	SysTick_init();
	DWT_init();

	UART_init(9600);
	gpio_init();
	SPI1_init();
	ST7735_init();
	RTC_init();

	ST7735_FillRect(0,0, 128, 160, 0x0000);
	ST7735_DrawString(3,3, "ST7735 INIT", 0xFFFF);

	CLEAR_BIT(GPIOC->ODR, (1 << 13));

uint32_t start[3] = {0}; // нулевые стартовые значения 
						 // для неблокирующих задержек
uint8_t screen_state = 0;

 while( 1 )
	{
	USART_commands();
	usart1_echo();
	if (ms_ticks - start[1] >= 1000)
	{
	start[1] = ms_ticks;
	screen_state ^= 1;

		if (screen_state) 	
		{
		ST7735_FillRect(108,140, 20, 20, 0xF800);
		ST7735_FillRect(40,70, 80, 30, 0x0000);
		print_Time();

		} else {
		ST7735_FillRect(108,140, 20, 20, 0x00FF);
		ST7735_FillRect(40,70, 80, 30, 0x0000);
		print_Time();

		}
	}

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

