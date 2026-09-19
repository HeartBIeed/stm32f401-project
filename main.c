#include "main.h"

modbus_rtu modbus1;

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
	USART1_send_string("start \r\n"); 

uint32_t start[3] = {0}; // нулевые стартовые значения 
						 // для неблокирующих задержек


modbus_init(&modbus1, 1); // инит адрес устройства 1

holding_registers[10] = 555;
holding_registers[11] = 777;

 while(1){

    modbus_poll(&modbus1);

	if (holding_registers[0] == 1)
	{
		hour = holding_registers[1];
		min = holding_registers[2];

		RTC_setTime(hour,min);
		
		holding_registers[0] = 0;
	}


  


if (ms_ticks - start[1] >= 1000) 
{


holding_registers[10] ++;
holding_registers[11] ++;

		RTC_getTime(&hour,&min,&sec);

		holding_registers[12] = hour;
		holding_registers[13] = min;
		holding_registers[14] = sec;
		
if (holding_registers[10]>777 && 
	holding_registers[11]>999 ){

		holding_registers[10] = 555;
		holding_registers[11] = 777;




}


	start[1] = ms_ticks;
}







if (ms_ticks - start[0] >= 1000) 
	{
		GPIOC->ODR ^= (1 << 13);
		start[0] = ms_ticks;
		RTC_print_time();
	}


	}
}

