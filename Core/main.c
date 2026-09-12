#include "main.h"
#include "rcc.h"
#include "uart.h"
#include "timer.h"
#include "spi.h"
#include "st7735_gfx.h"
#include "rtc.h"
#include "tm1638.h"
#include "gpio.h"
#include "clock.h"

//#include "i2c.h"
//#include "aht20.h"

uint32_t start[3] = {0}; 
void blink();

int main(void)
{
	SystemClock_PLL_84MHz();
	SysTick_init();
	DWT_init();

	GPIO_init();
	USART1_init(9600);
	SPI1_init();
	RTC_init();

	ST7735_init();
	tm1638_init();


	ST7735_FillRect(0,0,160,128,BLACK);
	USART1_send_string("start \r\n"); 

//	RTC_setTime(17,33);

while(1){






blink();
	}
}

void blink(){

	if (ms_ticks - start[0] >= 1000) 
		{
			GPIOC->ODR ^= (1 << 13);
			start[0] = ms_ticks;
			RTC_print_time();
		}
}