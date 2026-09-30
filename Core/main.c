#include "main.h"
#include "rcc.h"
#include "uart.h"
#include "timer.h"
#include "spi.h"
#include "rtc.h"
#include "gpio.h"

#include "eink.h"

uint8_t empty[10000];

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

			EINK_init();
				_delay_ms(1000);
			EINK_draw(empty);	

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
		}
}