#include "rcc.h"

void SystemClock_HSI_8MHz(void)
    {
    RCC->CR |=RCC_CR_HSION; // enable HSI
    while(!(RCC->CR & RCC_CR_HSIRDY)); // wait stable HSI

    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |=RCC_CFGR_SW_HSI; // HSI source SYSCLK
    //SW 0b00 HSI, 0b01-HSE, 0b10-PLL
    while((RCC->CFGR & RCC_CFGR_SWS)!=RCC_CFGR_SWS_HSI);//SWS status CFGR ->HSI EN

    }


void SystemClock_HSE(void) // 25 MHz BlackPill
    {
 //   RCC->CR |= RCC_CR_HSEBYP;  // включаем bypass
    RCC->CR |=RCC_CR_HSEON; // enable HSE
    while(!(RCC->CR & RCC_CR_HSERDY)); // wait stable HSE

    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |=RCC_CFGR_SW_HSE; // HSE source SYSCLK
    while((RCC->CFGR & RCC_CFGR_SWS)!=RCC_CFGR_SWS_HSE);//SWS status CFGR ->HSE EN

    }

void PLL_enable() //84 MHz from HSE 25 MHz
    {
 //   RCC->CR |= RCC_CR_HSEBYP;  // включаем bypass
    RCC->CR |=RCC_CR_HSEON; // enable HSE
    while(!(RCC->CR & RCC_CR_HSERDY)); // wait stable HSE


    RCC->PLLCFGR = (25<< RCC_PLLCFGR_PLLM_Pos) | //m div
                (336<< RCC_PLLCFGR_PLLN_Pos) | // N mul
                (1<< RCC_PLLCFGR_PLLP_Pos) | // P = 4
                (7<< RCC_PLLCFGR_PLLQ_Pos) | // Q = 7
                (1<< RCC_PLLCFGR_PLLSRC_Pos); //source - HSE

    // PLL enable                  
    RCC->CR |= RCC_CR_PLLON;
    while(!(RCC->CR & RCC_CR_PLLRDY)); // wait PLL is set

    FLASH->ACR |= FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_2WS; // cache / 2 tact flash

    //div bus
    RCC->CFGR |= (0b0<<RCC_CFGR_HPRE_Pos); // APB /1
    RCC->CFGR |= (0b101<<RCC_CFGR_PPRE1_Pos); // APB1 /4
    RCC->CFGR |= (0b100<<RCC_CFGR_PPRE2_Pos); // APB2 /2
    
/* Делители:
    0b000	/1
    0b100	/2
    0b101	/4
    0b110	/8
    0b111	/16
*/
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL; //PLL as system clock
    while ((RCC->CFGR & RCC_CFGR_SWS)!=RCC_CFGR_SWS_PLL); //wait switched SC to PLL

    SystemCoreClockUpdate();

    }
