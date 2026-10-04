#include "main.h"
#include "rcc.h"
#include "uart.h"
#include "timer.h"
#include "spi.h"
#include "rtc.h"
#include "gpio.h"

#include "eink.h"
#include "eink_gfx.h"

uint8_t framebuffer_eink[10000];

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
//			EINK_fill(EINK_FILL_YELLOW);	
//			EINK_fill(EINK_FILL_RED);	

	USART1_send_string("start \r\n"); 
//	RTC_setTime(17,33);

char str[] = {"Lorem ipsum dolor sit amet,"
	"consectetur adipiscing elit. Sed do "
	 "eiusmod tempor incididunt ut labore et"
	 "dolore magna aliqua. Ut enim ad minim veniam,"
	  "quis nostrud exercitation ullamco laboris nisi"
	  "ut aliquip ex ea commodo consequat."
	  "Duis aute irure dolor in reprehenderit"
	  "in voluptate velit esse cillum dolore eu"
	  "fugiat nulla pariatur. Excepteur sint occaecat"
	  "cupidatat non proident, sunt in culpa qui officia"
	  "deserunt mollit anim id est laborum."};

FB_FillRect(framebuffer_eink,0, 0, 200, 200, EINK_BLACK);
//FB_FillRect(framebuffer_eink,10, 10, 50, 50, EINK_RED);

bresenhamCircleFill(framebuffer_eink,160,160,30, EINK_RED);
//FB_DrawLine(framebuffer_eink,190,0,190, 200,EINK_WHITE);
FB_DrawString(framebuffer_eink,10, 10, str,EINK_YELLOW,Font_7x10);

EINK_draw(framebuffer_eink);

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