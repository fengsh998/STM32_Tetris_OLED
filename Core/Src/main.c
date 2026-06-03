#include "main.h"
#include "debug_log.h"
#include "gpio.h"
#include "spi.h"
#include "oled.h"
#include "tetris.h"
#include "graphics.h"

/*
 *
 * 7针OLED典型接线：
 *    VCC 		- 3.3V电源
 *    GND 		- 地
 *    SCL(D0) 	- SPI时钟（PA5）
 *    SDA(D1)	- SPI数据/MOSI（PA7）
 *    RES 		- 复位脚（PA4）
 *     DC 		- 数据/命令选择脚（PA3）
 *     CS 		- 片选脚（PA2）
 *
 *
 */

int main(void)
{
	//系统时钟配置
	SystemClock_Config();
	//滴塔时钟初始化
	SysTick_Init_ms();
	/* 初始化调试UART */
//	debug_uart_init();

	uint32_t last_refresh = 0;
	uint8_t start_button_pressed = 0;
	uint8_t screen_needs_update = 1;

	GPIO_Init();            /* Initialize GPIO pins */
	EXTI_Init();            /* Initialize external interrupts for buttons */
	SPI_Init();             /* Initialize SPI1 */
	Graphics_Init();        /* Initialize graphics and OLED */
	/* Set initial game state */
	game.game_state = GAME_STATE_MENU;

	/*  OLED 亮屏调试
	OLED_Init();
	//刷全屏白屏或者画个字符串来测试亮屏
	OLED_DrawString(0, 0, "OLED Test OK!", 1);
	//甚至可以画个矩形占满屏幕来看看
	OLED_DrawRect(0, 0, 128, 64, 1, 0);
	//必须调用 Refresh 才会真正通过 SPI 传给 OLED 芯片
	OLED_Refresh();
	*/

	while (1)
	{
		if (game.game_state == GAME_STATE_MENU) {
			/* 读取4个按键的状态（上拉输入：没按是1，按下是0） */
			uint8_t btn0 = GPIO_ReadPin(GPIOB, 0);
			uint8_t btn1 = GPIO_ReadPin(GPIOB, 1);
			uint8_t btn3 = GPIO_ReadPin(GPIOB, 3);
			uint8_t btn4 = GPIO_ReadPin(GPIOB, 4);

			/* 核心修复：只要其中任意一个按键等于 0，就说明有按键被按下了，即任意键开始游戏 */
			if (btn0 == 0 || btn1 == 0 || btn3 == 0 || btn4 == 0) {
				if (!start_button_pressed) {
					start_button_pressed = 1;
					Tetris_Init(); /* 初始化/开始新游戏 */
					Graphics_RefreshDisplay(); /* 绘制初始游戏画面 */
					last_refresh = get_tick_ms();
					screen_needs_update = 1;
				}
			} else {
				/* 所有按键都等于 1（即全部松开）时，重置标志位 */
				start_button_pressed = 0;
			}
		} else if (game.game_state == GAME_STATE_PLAYING) {
			/* Update game logic */
			Tetris_Update();

			/* Refresh display every 50ms */
			if ((get_tick_ms() - last_refresh) >= 50) {
				Graphics_RefreshDisplay();
				last_refresh = get_tick_ms();
			}
		} else if (game.game_state == GAME_STATE_GAMEOVER) {
			/* Display game over screen */
			if (screen_needs_update) {
				Graphics_DrawGameOver(); // 只在状态切换时调用一次
				screen_needs_update = 0; // 停止频繁重绘
			}

			/* 读取4个按键的状态 */
			uint8_t btn0 = GPIO_ReadPin(GPIOB, 0);
			uint8_t btn1 = GPIO_ReadPin(GPIOB, 1);
			uint8_t btn3 = GPIO_ReadPin(GPIOB, 3);
			uint8_t btn4 = GPIO_ReadPin(GPIOB, 4);

			/* 核心修复：结束界面同样，任意键按下返回菜单 */
			if (btn0 == 0 || btn1 == 0 || btn3 == 0 || btn4 == 0) {
				if (!start_button_pressed) {
					game.game_state = GAME_STATE_MENU;
					Graphics_DrawMenu();
					screen_needs_update = 1;
					start_button_pressed = 1;
				}
			} else {
				start_button_pressed = 0;
			}
		}
	}
}



void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{

}
#endif
