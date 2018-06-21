#include "ustime.h"
#include <FreeRTOS.h>
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"

/**
 * Perform a microsecond delay
 *
 * \param  us  number of microseconds to wait.
 * \note   The actual delay will last between us and (us-1) microseconds.
 *         To wait _at_least_ 1 us, you should use delay_us(2).
 */
void delayUs(unsigned long us)
{
    uint16_t  t0 = TIM2->CNT;
    for (;;) {
        int  t = TIM2->CNT;
        if (t < t0)
            t += 0x10000;

        if (us < t - t0)
            return;

        us -= t - t0;
        t0  = t;
    }
}


/**
 * Performs a millisecond delay
 *
 * \param  ms  number of milliseconds to wait.
 */
void delayMs(unsigned long ms)
{
    delayUs(ms * 1000);
}

/**
 * Set up TIM2 as a 32bit, microsecond-timer.
 *
 */
void initUsTimer()
{
    RCC_ClocksTypeDef RCC_Clocks;
    RCC_GetClocksFreq(&RCC_Clocks);

    RCC->APB1ENR |= RCC_APB1Periph_TIM2;
    TIM2->PSC = (RCC_Clocks.PCLK2_Frequency / 1000000) - 1;
    TIM2->ARR = 0xFFFF;
    TIM2->CR1 = TIM_CR1_CEN;
}

