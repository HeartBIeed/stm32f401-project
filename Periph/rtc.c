#include "rtc.h"

void RTC_init(){
	
	RCC->APB1ENR |= (1<<28); // pwr en
	//Power control registers
	PWR->CR |=(1<<8); // DBP: Disable RTC domain write protection.
			_delay_us(5);

	RCC->BDCR |= (1<<0); //LSE on

	uint32_t timeout = TIMEOUT;
	while(!(RCC->BDCR & (1<<1))){ // wait LSE ready
        if(--timeout == 0) break; // Защита от зависания
	}

	//RTC domain control register (RCC_BDCR)
	RCC->BDCR &= ~(3<<8); 
	RCC->BDCR |= (1<<8); // LSE oscillator
	//01: LSE oscillator clock used as RTC clock

	RCC->BDCR |= (1<<15); // RTC clock enable

// *********************** BEGIN SET RTC ************************

	RTC->WPR = 0xCA; //Write access for RTC registers
	RTC->WPR = 0x53;  
	RTC->ISR |= RTC_ISR_INIT; // enable init phase 
	while(!(RTC->ISR & (1<<6))); // wait INITF ready
	//Bit 6 INITF: Initialization flag

	RTC->PRER = (127<<16) | 255; //Asynchronous//Synchronous

	RTC->ISR &= ~RTC_ISR_INIT; // Disable init phase 

	RTC->WPR = 0xFE; 
	RTC->WPR = 0x64; // Disable write access for RTC registers 
// *********************** END SET RTC **************************

}


void RTC_setTime(uint8_t h,uint8_t m){

	RTC->WPR = 0xCA; 
	RTC->WPR = 0x53;  

	RTC->ISR |= RTC_ISR_INIT; 
	while(!(RTC->ISR & (1<<6))); // wait INITF ready

	uint32_t tr =((h / 10) << 20) |
				((h % 10) << 16) |
				((m / 10) << 12) |
				((m % 10) << 8);

	RTC->TR = tr;

	RTC->ISR &= ~RTC_ISR_INIT; 
	RTC->WPR = 0xFE; 
	RTC->WPR = 0x64; 

}


void RTC_getTime(uint8_t *hour,uint8_t *min,uint8_t *sec){

	uint32_t tr = RTC->TR;

	*hour = ((tr>>20)&0x3)*10 + ((tr>>16)&0xF); // 0x3 read 2 bits
	*min =  ((tr>>12)&0x7)*10 + ((tr>>8)&0xF); // 0x7 read 3 bits
	*sec =  ((tr>>4)&0x7)*10 + ((tr>>0)&0xF); // 0x7 read 3 bits
											// 0xF read 4 bits
}


/*
void RTC_alarmSec()
{

EXTI->IMR |= EXTI_IMR_MR17; // Разрешим прерывание 17 линиии EXTI
EXTI->RTSR |= EXTI_RTSR_TR17; // Прерывание по восходящему фронту

// *********************** BEGIN SET RTC ************************

	RTC->WPR = 0xCA; 
	RTC->WPR = 0x53;  

	RTC->CR &=~ RTC_CR_ALRAE; 
	while (!(RTC->ISR & RTC_ISR_ALRAWF)); 

RTC->ALRMAR |= RTC_ALRMAR_MSK1 | RTC_ALRMAR_MSK2 | RTC_ALRMAR_MSK3 | RTC_ALRMAR_MSK4;
	            
	RTC->CR = RTC_CR_ALRAIE | RTC_CR_ALRAE; 

	RTC->WPR = 0xFE; 
	RTC->WPR = 0x64; 
// *********************** END SET RTC **************************

NVIC_SetPriority(RTC_IRQn,1);
NVIC_EnableIRQ(RTC_IRQn);

}
*/
