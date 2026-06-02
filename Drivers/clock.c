#include "stm32f10x.h"
#include "clock.h"

static volatile uint32_t systick_count = 0;

/**
 * Clock_Init - Initialize system clock to 72MHz using PLL
 * HSE (8MHz) -> PLL x9 -> SYSCLK (72MHz)
 * APB1 = 36MHz, APB2 = 72MHz
 */
void Clock_Init(void)
{
    /* Enable HSE (High Speed External clock) */
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY));
    
    /* Configure PLL */
    RCC->CFGR &= ~RCC_CFGR_PLLMULL;
    RCC->CFGR |= RCC_CFGR_PLLMULL9;  /* PLL x9 */
    RCC->CFGR |= RCC_CFGR_PLLSRC;    /* HSE as PLL source */
    
    /* Configure APB1 and APB2 prescalers */
    RCC->CFGR &= ~RCC_CFGR_PPRE1;
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;  /* APB1 = SYSCLK/2 = 36MHz */
    RCC->CFGR &= ~RCC_CFGR_PPRE2;
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;  /* APB2 = SYSCLK = 72MHz */
    
    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));
    
    /* Select PLL as system clock */
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
    
    /* Update SystemCoreClock */
    SystemCoreClock = SYSCLK_FREQ;
}

/**
 * SysTick_Init - Initialize SysTick timer for 1ms interrupt
 */
void SysTick_Init(void)
{
    /* Disable SysTick */
    SysTick->CTRL = 0;
    
    /* Set reload value: 72MHz / 1000 = 72000 ticks per 1ms */
    SysTick->LOAD = SYSCLK_FREQ / 1000 - 1;
    
    /* Clear current value */
    SysTick->VAL = 0;
    
    /* Set priority and enable interrupt */
    NVIC_SetPriority(SysTick_IRQn, 0);
    
    /* Enable SysTick: clock source = SYSCLK, enable interrupt */
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | 
                    SysTick_CTRL_TICKINT_Msk | 
                    SysTick_CTRL_ENABLE_Msk;
}

/**
 * SysTick_Handler - SysTick interrupt handler (1ms)
 */
void SysTick_Handler(void)
{
    systick_count++;
}

/**
 * Delay_ms - Blocking delay in milliseconds
 */
void Delay_ms(uint32_t ms)
{
    uint32_t start = systick_count;
    while ((systick_count - start) < ms);
}

/**
 * Delay_us - Blocking delay in microseconds
 */
void Delay_us(uint32_t us)
{
    uint32_t ticks = (SYSCLK_FREQ / 1000000) * us / 9;  /* Approximate for Cortex-M3 */
    while (ticks--);
}

/**
 * Get_Systick - Get current systick count
 */
uint32_t Get_Systick(void)
{
    return systick_count;
}
