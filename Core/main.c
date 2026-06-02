#include "stm32f10x.h"
#include "clock.h"
#include "gpio.h"
#include "spi.h"
#include "oled.h"
#include "tetris.h"
#include "graphics.h"

/**
 * main - Main program entry point
 */
int main(void)
{
    uint32_t last_refresh = 0;
    uint32_t game_menu_start = 0;
    uint8_t start_button_pressed = 0;
    
    /* Initialize system */
    Clock_Init();           /* Configure system clock to 72MHz */
    SysTick_Init();         /* Initialize 1ms systick timer */
    GPIO_Init();            /* Initialize GPIO pins */
    EXTI_Init();            /* Initialize external interrupts for buttons */
    SPI_Init();             /* Initialize SPI1 */
    Graphics_Init();        /* Initialize graphics and OLED */
    
    /* Wait for user to start game */
    game_menu_start = Get_Systick();
    
    while (1) {
        if (game.game_state == GAME_STATE_MENU) {
            /* Menu state: wait for any button press to start */
            uint8_t any_button = GPIO_ReadPin(GPIOB, 0) | 
                                 GPIO_ReadPin(GPIOB, 1) | 
                                 GPIO_ReadPin(GPIOB, 2) | 
                                 GPIO_ReadPin(GPIOB, 3);
            
            if (any_button == 0) {  /* Any button pressed */
                if (!start_button_pressed) {
                    start_button_pressed = 1;
                    Tetris_Init();              /* Start new game */
                    Graphics_RefreshDisplay();  /* Draw initial game screen */
                    last_refresh = Get_Systick();
                }
            } else {
                start_button_pressed = 0;
            }
        }
        else if (game.game_state == GAME_STATE_PLAYING) {
            /* Update game logic */
            Tetris_Update();
            
            /* Refresh display every 50ms */
            if ((Get_Systick() - last_refresh) >= 50) {
                Graphics_RefreshDisplay();
                last_refresh = Get_Systick();
            }
        }
        else if (game.game_state == GAME_STATE_GAMEOVER) {
            /* Display game over screen */
            Graphics_DrawGameOver();
            
            /* Wait for button press to return to menu */
            uint8_t any_button = GPIO_ReadPin(GPIOB, 0) | 
                                 GPIO_ReadPin(GPIOB, 1) | 
                                 GPIO_ReadPin(GPIOB, 2) | 
                                 GPIO_ReadPin(GPIOB, 3);
            
            if (any_button == 0) {  /* Any button pressed */
                if (!start_button_pressed) {
                    start_button_pressed = 1;
                    game.game_state = GAME_STATE_MENU;
                    Graphics_DrawMenu();
                }
            } else {
                start_button_pressed = 0;
            }
        }
    }
    
    return 0;
}
