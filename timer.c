#include "timer.h"

volatile uint32_t ms_ticks = 0;

void SysTick_init()
    {
    SystemCoreClockUpdate();

    SysTick->LOAD =(SystemCoreClock/1000) - 1; // 1 ms
    SysTick->VAL = 0; //clear
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk| //source AHB
    SysTick_CTRL_TICKINT_Msk | //interrupt
    SysTick_CTRL_ENABLE_Msk; 

    }

void DWT_init()
    {
        // debug 
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // trace EN
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; // EN cycle counter 

    }


void SysTick_Handler()
    {
        ms_ticks++;

    }

uint32_t get_mills() // счетчик мс
    {
        return ms_ticks;
    }

void _delay_ms(uint32_t ms)
    {
    uint32_t start = ms_ticks;
    while (( ms_ticks - start) < ms);

    }

void _delay_us(uint32_t us)
    {
    uint32_t start = DWT->CYCCNT;
    uint32_t ticks = us * (SystemCoreClock/1000000);
    while ((DWT->CYCCNT - start)<ticks);

    }
