#include "main.h"

#define HOLDING_REGISTERS_COUNT 10U

modbus_rtu modbus1;

static uint16_t holding_registers[HOLDING_REGISTERS_COUNT] = {
    100U,
    200U,
    300U,
    400U,
    500U,
    600U,
    700U,
    800U,
    900U,
    1000U
};



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


modbus_init(&modbus1, 1U); // инит адрес 1


 while(1){
	USART1_send_string("start \r\n"); 


    modbus_poll(&modbus1);

	if (modbus1.rx_state == MODBUS_RX_FRAME_READY) {
	
		if (modbus_frame_check(&modbus1)) {

			uint8_t modbus1_function = modbus1.rx_frame[1];
	
			if (modbus1_function == 0x03U) {
///******************************************************
					
			if (modbus_handle_read_holding(&modbus1,holding_registers,HOLDING_REGISTERS_COUNT) != 0) {

                for (uint16_t i = 0U; i < modbus1.tx_length; i++) {

                USART1_send_byte(modbus1.tx_frame[i]);
                }
            }
//******************************* */
			}
		}
	
		modbus_frame_reset(&modbus1);
	}



/*

rx_frame[0] == 0x01  // адрес Slave
rx_frame[1] == 0x03  // Read Holding Registers
rx_frame[2] == 0x00  // старший байт начального адреса
rx_frame[3] == 0x00  // младший байт начального адреса
rx_frame[4] == 0x00  // старший байт количества
rx_frame[5] == 0x0A  // прочитать 10 регистровЫ

*/

if (ms_ticks - start[1] >= 10) 
{

    if (modbus1.rx_state == MODBUS_RX_FRAME_READY) { // если состояние в пулле ГОТОВ
        /*
         * Здесь позже будет проверка адреса, длины и CRC.
         */

        modbus1.rx_length = 0U; // обнуляем длину
        modbus1.rx_state = MODBUS_RX_IDLE; // ОЖИДАНИЕ
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

