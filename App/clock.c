#include "clock.h"
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

/*	sprintf(text, "%02d:%02d:%02d",hour,min,sec);
	ST7796_FillRect(20,20,160,110,BLACK);
	ST7796_DrawString(20,20,text, YELLOW,Font_11x18);
	*/
}