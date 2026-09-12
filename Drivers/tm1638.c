#include "tm1638.h"

#define STB 15
#define CLK 12
#define DIO 11

#define STB_UP GPIOA->BSRR |= (1 << STB)
#define STB_DOWN GPIOA->BSRR |= (1 << (STB + 16))

#define D1 GPIOA->BSRR |= (1 << DIO)
#define D0 GPIOA->BSRR |= (1 << (DIO + 16))	

void clk(){

	GPIOA->BSRR |= (1 << (CLK + 16));
		_delay_ms(1);
	GPIOA->BSRR |= (1 << CLK);	
		_delay_ms(1);
}

void tm1638_send(uint8_t data){
	
	SET_BIT(GPIOA->MODER,(1 << (DIO*2)));

	for (int i = 0; i < 8; i++){
			
		if (data&(1<<0)) D1;
		else			 D0;

		clk();
			
		data >>= 1; 
	}
}

uint8_t tm1638_read(){


	STB_DOWN;
	tm1638_send(0x42); //read button
	STB_UP;

	int data = 0;

	CLEAR_BIT(GPIOA->MODER,(1 << (DIO*2)));

	STB_DOWN;


	for (int i = 0; i < 8; i++)
	{
			
		if (READ_BIT(GPIOA->IDR, (1 << DIO))) data |= 1; //+1 if pressed	
		data <<= 1;

		clk();
	}

	STB_UP;

	return data;
}

void tm1638_init(){

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 

	MODIFY_REG(GPIOA->MODER, (3 << (STB*2)), (1 << (STB*2)));
	SET_BIT(GPIOA->MODER,(1 << (CLK*2)));
	SET_BIT(GPIOA->MODER,(1 << (DIO*2)));

	STB_UP;
		_delay_ms(1);
		
	STB_DOWN;	
	tm1638_send(0x88); //low brightness
	STB_UP;
		
	STB_DOWN;
	tm1638_send(0x44); //non increment address
	STB_UP;
}

void tm1638_set_digit(uint8_t digit,uint8_t position, uint8_t dot){

	const uint8_t digits[10] = {

		0x3F, // 0
		0x06, // 1
		0x5B, // 2
		0x4F, // 3
		0x66, // 4
		0x6D, // 5
		0x7D, // 6
		0x07, // 7
		0x7F, // 8
		0x6F  // 9
	};

	uint8_t data = digits[digit];
	
	if (dot) data |= 0x80; // set dot under digit | 0b10000000;
		
	STB_DOWN;

	uint8_t digit_addr = position * 2;

	tm1638_send(0xC0 | digit_addr); // set position
	tm1638_send(data); // send digit

	STB_UP;
}

void tm1638_set_led(uint8_t state, uint8_t position){

	STB_DOWN;

	uint8_t led_addr = position * 2 + 1;

	tm1638_send(0xC0 | led_addr); // set position
	tm1638_send(state); // send led state

	STB_UP;
}



/*
	tm1638_init(0);

	tm1638_set_digit(1,0,0);
	tm1638_set_digit(2,1,1);
	tm1638_set_digit(3,2,1);
	tm1638_set_digit(4,3,0);
	tm1638_set_digit(5,4,0);
	tm1638_set_digit(6,5,1);
	tm1638_set_digit(7,6,1);
	tm1638_set_digit(8,7,0);



for (int i = 0; i < 8; i++)
{
	tm1638_set_led(1, i);
	tm1638_set_led(0, i--);
		_delay_ms(1000);
}

*/
