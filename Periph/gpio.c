#include "gpio.h"

void GPIO_init()
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; 
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; 

    GPIOC->MODER |= ( 1 << (13*2)); //PC13 / output 01

}