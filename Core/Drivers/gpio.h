#ifndef __GPIO_H
#define __GPIO_H

#include <stdint.h>

/* SPI Interface Pins (OLED) */
#define OLED_CS_PORT    GPIOA
#define OLED_CS_PIN     2
#define OLED_DC_PORT    GPIOA
#define OLED_DC_PIN     3
#define OLED_RES_PORT   GPIOA
#define OLED_RES_PIN    4

/* SPI Pins */
#define SPI_SCL_PORT    GPIOA      /* PA5 - SCL */
#define SPI_SCL_PIN     5
#define SPI_SDA_PORT    GPIOA      /* PA7 - SDA/MOSI */
#define SPI_SDA_PIN     7

/* Button Pins */
#define BTN_LEFT_PORT   GPIOB
#define BTN_LEFT_PIN    0          /* PB0 */
#define BTN_RIGHT_PORT  GPIOB
#define BTN_RIGHT_PIN   1          /* PB1 */
#define BTN_DOWN_PORT   GPIOB
#define BTN_DOWN_PIN    4          /* PB4 */
#define BTN_ROTATE_PORT GPIOB
#define BTN_ROTATE_PIN  3          /* PB3 */

/* GPIO Functions */
void GPIO_Init(void);
void GPIO_SetPin(GPIO_TypeDef* port, uint16_t pin);
void GPIO_ResetPin(GPIO_TypeDef* port, uint16_t pin);
uint8_t GPIO_ReadPin(GPIO_TypeDef* port, uint16_t pin);
void EXTI_Init(void);

#endif
