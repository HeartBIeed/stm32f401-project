#include "clock.h"
#include "tm1638.h"
#include "rtc.h"
#include "uart.h"

uint8_t hour;
uint8_t min;
uint8_t sec;


void RTC_print_time()
{

	RTC_getTime(&hour,&min,&sec);

	char text[20]={"\0"};	
	sprintf(text, "TIME %02d:%02d:%02d \r\n",hour,min,sec);
	USART1_send_string(text); 

	uint8_t dot;

	uint8_t tens_h  = (hour / 10)   % 10; 
	uint8_t units_h = hour % 10;         

	uint8_t tens_m  = (min / 10)   % 10; 
	uint8_t units_m = min % 10;

	if (READ_BIT(GPIOC->ODR, (1 << 13)))
	{
		 dot = 1;
	} else {
		 dot = 0;
	}

	tm1638_set_digit(tens_h,0,0);
	tm1638_set_digit(units_h,1,dot);
	tm1638_set_digit(tens_m,2,0);
	tm1638_set_digit(units_m,3,0);

/*	sprintf(text, "%02d:%02d:%02d",hour,min,sec);
	ST7735_FillRect(20,20,160,110,BLACK);
	ST7735_DrawString(20,20,text, YELLOW,Font_11x18);
	*/
}