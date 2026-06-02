#ifndef __CLOCK_H
#define __CLOCK_H

#include <stdint.h>

/* Clock Configuration */
#define HSE_VALUE    8000000    /* External oscillator frequency: 8MHz */
#define PLL_MUL      9          /* PLL multiplication factor: 72MHz = 8MHz * 9 */
#define SYSCLK_FREQ  72000000   /* System clock: 72MHz */

void Clock_Init(void);
void SysTick_Init(void);
void Delay_ms(uint32_t ms);
void Delay_us(uint32_t us);

#endif
