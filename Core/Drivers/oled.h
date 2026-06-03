#ifndef __OLED_H
#define __OLED_H

#include <stdint.h>

/* OLED Display Parameters */
#define OLED_WIDTH      128
#define OLED_HEIGHT     64
#define OLED_PAGES      (OLED_HEIGHT / 8)

/* OLED Commands */
#define OLED_CMD_CONTRAST           0x81
#define OLED_CMD_ENTIRE_ON_RESUME   0xA4
#define OLED_CMD_ENTIRE_ON          0xA5
#define OLED_CMD_NORMAL_DISPLAY     0xA6
#define OLED_CMD_INVERT_DISPLAY     0xA7
#define OLED_CMD_DISPLAY_OFF        0xAE
#define OLED_CMD_DISPLAY_ON         0xAF
#define OLED_CMD_SET_PAGE_ADDR      0xB0
#define OLED_CMD_SET_COL_ADDR_L     0x00
#define OLED_CMD_SET_COL_ADDR_H     0x10
#define OLED_CMD_START_LINE         0x40

/* OLED Functions */
void OLED_Init(void);
void OLED_WriteCommand(uint8_t cmd);
void OLED_WriteData(uint8_t data);
void OLED_Clear(void);
void OLED_DisplayON(void);
void OLED_DisplayOFF(void);
void OLED_SetContrast(uint8_t contrast);
void OLED_SetPosition(uint8_t page, uint8_t col);
void OLED_DrawPixel(uint16_t x, uint16_t y, uint8_t color);
void OLED_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color);
void OLED_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t color, uint8_t fill);
void OLED_DrawChar(uint16_t x, uint16_t y, char ch, uint8_t color);
void OLED_DrawString(uint16_t x, uint16_t y, const char* str, uint8_t color);
void OLED_Refresh(void);

/* Frame buffer (1024 bytes for 128x64 display) */
extern uint8_t oled_buffer[OLED_PAGES][OLED_WIDTH];

#endif
