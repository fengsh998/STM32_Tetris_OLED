#include "stm32f1xx.h"
#include "tetris.h"
#include "graphics.h"
#include "gpio.h"
#include "systick.h"


/* Button debounce time in ms */
#define DEBOUNCE_TIME 20

/* Global button press flags */
volatile uint8_t btn_left_pressed = 0;
volatile uint8_t btn_right_pressed = 0;
volatile uint8_t btn_down_pressed = 0;
volatile uint8_t btn_rotate_pressed = 0;

/**
 * EXTI0_IRQHandler - Left button interrupt (PB0)
 */
void EXTI0_IRQHandler(void)
{
    if (EXTI->PR & EXTI_PR_PR0) {
        EXTI->PR = EXTI_PR_PR0;  /* Clear pending bit */
        delay_ms(DEBOUNCE_TIME);
        
        if (GPIO_ReadPin(GPIOB, 0) == 0) {  /* Button pressed (active low) */
            btn_left_pressed = 1;
            Tetris_MoveLeft();
        }
    }
}

/**
 * EXTI1_IRQHandler - Right button interrupt (PB1)
 */
void EXTI1_IRQHandler(void)
{
    if (EXTI->PR & EXTI_PR_PR1) {
        EXTI->PR = EXTI_PR_PR1;  /* Clear pending bit */
        delay_ms(DEBOUNCE_TIME);
        
        if (GPIO_ReadPin(GPIOB, 1) == 0) {  /* Button pressed (active low) */
            btn_right_pressed = 1;
            Tetris_MoveRight();
        }
    }
}

/**
 * EXTI2_IRQHandler - Down button interrupt (PB4)
 */
void EXTI4_IRQHandler(void)
{
    if (EXTI->PR & EXTI_PR_PR4) {
        EXTI->PR = EXTI_PR_PR4;  /* Clear pending bit */
        delay_ms(DEBOUNCE_TIME);
        
        if (GPIO_ReadPin(GPIOB, 2) == 0) {  /* Button pressed (active low) */
            btn_down_pressed = 1;
            Tetris_MoveDown();
        }
    }
}

/**
 * EXTI3_IRQHandler - Rotate button interrupt (PB3)
 */
void EXTI3_IRQHandler(void)
{
    if (EXTI->PR & EXTI_PR_PR3) {
        EXTI->PR = EXTI_PR_PR3;  /* Clear pending bit */
        delay_ms(DEBOUNCE_TIME);
        
        if (GPIO_ReadPin(GPIOB, 3) == 0) {  /* Button pressed (active low) */
            btn_rotate_pressed = 1;
            Tetris_Rotate();
        }
    }
}
