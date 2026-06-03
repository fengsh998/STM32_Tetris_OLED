#include "stm32f1xx.h"
#include "oled.h"
#include "spi.h"
#include "gpio.h"
#include "systick.h"

/* Frame buffer: 8 pages x 128 columns */
uint8_t oled_buffer[OLED_PAGES][OLED_WIDTH];

/* 5x8 font for characters */
const uint8_t font_5x8[96][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, /* space */
    {0x00, 0x00, 0x5F, 0x00, 0x00}, /* ! */
    {0x00, 0x07, 0x00, 0x07, 0x00}, /* " */
    {0x14, 0x7F, 0x14, 0x7F, 0x14}, /* # */
    {0x24, 0x2A, 0x7F, 0x2A, 0x12}, /* $ */
    {0x23, 0x13, 0x08, 0x64, 0x62}, /* % */
    {0x36, 0x49, 0x55, 0x22, 0x50}, /* & */
    {0x00, 0x05, 0x03, 0x00, 0x00}, /* ' */
    {0x00, 0x1C, 0x22, 0x41, 0x00}, /* ( */
    {0x00, 0x41, 0x22, 0x1C, 0x00}, /* ) */
    {0x14, 0x08, 0x3E, 0x08, 0x14}, /* * */
    {0x08, 0x08, 0x3E, 0x08, 0x08}, /* + */
    {0x00, 0x50, 0x30, 0x00, 0x00}, /* , */
    {0x08, 0x08, 0x08, 0x08, 0x08}, /* - */
    {0x00, 0x60, 0x60, 0x00, 0x00}, /* . */
    {0x20, 0x10, 0x08, 0x04, 0x02}, /* / */
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, /* 0 */
    {0x00, 0x42, 0x7F, 0x40, 0x00}, /* 1 */
    {0x42, 0x61, 0x51, 0x49, 0x46}, /* 2 */
    {0x21, 0x41, 0x45, 0x4B, 0x31}, /* 3 */
    {0x18, 0x14, 0x12, 0x7F, 0x10}, /* 4 */
    {0x27, 0x45, 0x45, 0x45, 0x39}, /* 5 */
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, /* 6 */
    {0x01, 0x71, 0x09, 0x05, 0x03}, /* 7 */
    {0x36, 0x49, 0x49, 0x49, 0x36}, /* 8 */
    {0x06, 0x49, 0x49, 0x29, 0x1E}, /* 9 */
    {0x00, 0x36, 0x36, 0x00, 0x00}, /* : */
    {0x00, 0x56, 0x36, 0x00, 0x00}, /* ; */
    {0x08, 0x14, 0x22, 0x41, 0x00}, /* < */
    {0x14, 0x14, 0x14, 0x14, 0x14}, /* = */
    {0x00, 0x41, 0x22, 0x14, 0x08}, /* > */
    {0x02, 0x01, 0x51, 0x09, 0x06}, /* ? */
    {0x32, 0x49, 0x79, 0x41, 0x3E}, /* @ */
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, /* A */
    {0x7F, 0x49, 0x49, 0x49, 0x36}, /* B */
    {0x3E, 0x41, 0x41, 0x41, 0x22}, /* C */
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, /* D */
    {0x7F, 0x49, 0x49, 0x49, 0x41}, /* E */
    {0x7F, 0x09, 0x09, 0x09, 0x01}, /* F */
    {0x3E, 0x41, 0x49, 0x49, 0x7A}, /* G */
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, /* H */
    {0x00, 0x41, 0x7F, 0x41, 0x00}, /* I */
    {0x20, 0x40, 0x41, 0x3F, 0x01}, /* J */
    {0x7F, 0x08, 0x14, 0x22, 0x41}, /* K */
    {0x7F, 0x40, 0x40, 0x40, 0x40}, /* L */
    {0x7F, 0x02, 0x0C, 0x02, 0x7F}, /* M */
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, /* N */
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, /* O */
    {0x7F, 0x09, 0x09, 0x09, 0x06}, /* P */
    {0x3E, 0x41, 0x51, 0x21, 0x5E}, /* Q */
    {0x7F, 0x09, 0x19, 0x29, 0x46}, /* R */
    {0x46, 0x49, 0x49, 0x49, 0x31}, /* S */
    {0x01, 0x01, 0x7F, 0x01, 0x01}, /* T */
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, /* U */
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, /* V */
    {0x3F, 0x40, 0x38, 0x40, 0x3F}, /* W */
    {0x63, 0x14, 0x08, 0x14, 0x63}, /* X */
    {0x07, 0x08, 0x70, 0x08, 0x07}, /* Y */
    {0x61, 0x51, 0x49, 0x45, 0x43}, /* Z */
    {0x00, 0x7F, 0x41, 0x41, 0x00}, /* [ */
    {0x02, 0x04, 0x08, 0x10, 0x20}, /* \ */
    {0x00, 0x41, 0x41, 0x7F, 0x00}, /* ] */
    {0x04, 0x02, 0x01, 0x02, 0x04}, /* ^ */
    {0x40, 0x40, 0x40, 0x40, 0x40}, /* _ */
    {0x00, 0x01, 0x02, 0x04, 0x00}, /* ` */
    {0x20, 0x54, 0x54, 0x54, 0x78}, /* a */
    {0x7F, 0x48, 0x44, 0x44, 0x38}, /* b */
    {0x38, 0x44, 0x44, 0x44, 0x20}, /* c */
    {0x38, 0x44, 0x44, 0x48, 0x7F}, /* d */
    {0x38, 0x54, 0x54, 0x54, 0x18}, /* e */
    {0x08, 0x7E, 0x09, 0x01, 0x02}, /* f */
    {0x0C, 0x52, 0x52, 0x52, 0x3E}, /* g */
    {0x7F, 0x08, 0x04, 0x04, 0x78}, /* h */
    {0x00, 0x44, 0x7D, 0x40, 0x00}, /* i */
    {0x20, 0x40, 0x44, 0x3D, 0x00}, /* j */
    {0x7F, 0x10, 0x28, 0x44, 0x00}, /* k */
    {0x00, 0x41, 0x7F, 0x40, 0x00}, /* l */
    {0x78, 0x04, 0x18, 0x04, 0x78}, /* m */
    {0x78, 0x04, 0x04, 0x04, 0x78}, /* n */
    {0x38, 0x44, 0x44, 0x44, 0x38}, /* o */
    {0x7C, 0x14, 0x14, 0x14, 0x08}, /* p */
    {0x08, 0x14, 0x14, 0x18, 0x7C}, /* q */
    {0x7C, 0x08, 0x04, 0x04, 0x08}, /* r */
    {0x48, 0x54, 0x54, 0x54, 0x20}, /* s */
    {0x04, 0x3F, 0x44, 0x40, 0x20}, /* t */
    {0x3C, 0x40, 0x40, 0x20, 0x7C}, /* u */
    {0x1C, 0x20, 0x40, 0x20, 0x1C}, /* v */
    {0x3C, 0x40, 0x30, 0x40, 0x3C}, /* w */
    {0x44, 0x28, 0x10, 0x28, 0x44}, /* x */
    {0x0C, 0x50, 0x50, 0x50, 0x3C}, /* y */
    {0x44, 0x64, 0x54, 0x4C, 0x44}, /* z */
    {0x00, 0x08, 0x36, 0x41, 0x00}, /* { */
    {0x00, 0x00, 0x7F, 0x00, 0x00}, /* | */
    {0x00, 0x41, 0x36, 0x08, 0x00}, /* } */
    {0x10, 0x08, 0x08, 0x10, 0x08}, /* ~ */
};

/**
 * OLED_Init - Initialize OLED display (SSD1306)
 */
void OLED_Init(void)
{
    /* Reset sequence */
    GPIO_ResetPin(GPIOA, 4);        /* RES = 0 */
    delay_ms(100);
    GPIO_SetPin(GPIOA, 4);          /* RES = 1 */
    delay_ms(100);
    
    /* Initialize display */
    OLED_WriteCommand(0xAE); // Display off
    OLED_WriteCommand(0xD5); // Set display clock divide ratio/oscillator freq
    OLED_WriteCommand(0x80);
	OLED_WriteCommand(0xA8); // Set multiplex ratio(1 to 64)
	OLED_WriteCommand(0x3F);
	OLED_WriteCommand(0xD3); // Set display offset
	OLED_WriteCommand(0x00);
	OLED_WriteCommand(0x40); // Set start line address
	OLED_WriteCommand(0x8D); // Charge Pump
	OLED_WriteCommand(0x14); // Enable charge pump
	OLED_WriteCommand(0x20); // Memory addressing mode
	OLED_WriteCommand(0x00); // Horizontal addressing mode
	OLED_WriteCommand(0xA1); // Segment remap
	OLED_WriteCommand(0xC8); // COM scan direction remapped
	OLED_WriteCommand(0xDA); // COM pins hardware config
	OLED_WriteCommand(0x12);
	OLED_WriteCommand(0x81); // Contrast
	OLED_WriteCommand(0x7F);
	OLED_WriteCommand(0xD9); // Pre-charge
	OLED_WriteCommand(0xF1);
	OLED_WriteCommand(0xDB); // VCOMH deselect level
	OLED_WriteCommand(0x40);
	OLED_WriteCommand(0xA4); // Resume to RAM content display
	OLED_WriteCommand(0xA6); // Normal display

    OLED_Clear();
    OLED_WriteCommand(0xAF);        /* Display ON */
}

/**
 * OLED_WriteCommand - Write command to OLED
 */
void OLED_WriteCommand(uint8_t cmd)
{
    GPIO_ResetPin(GPIOA, 2);        /* CS = 0 */
    GPIO_ResetPin(GPIOA, 3);        /* DC = 0 (command mode) */
    SPI_Write(cmd);
    GPIO_SetPin(GPIOA, 2);          /* CS = 1 */
}

/**
 * OLED_WriteData - Write data to OLED
 */
void OLED_WriteData(uint8_t data)
{
    GPIO_ResetPin(GPIOA, 2);        /* CS = 0 */
    GPIO_SetPin(GPIOA, 3);          /* DC = 1 (data mode) */
    SPI_Write(data);
    GPIO_SetPin(GPIOA, 2);          /* CS = 1 */
}

/**
 * OLED_SetPosition - Set cursor position (page and column)
 * page: 0-7
 * col: 0-127
 */
void OLED_SetPosition(uint8_t page, uint8_t col)
{
    OLED_WriteCommand(0xB0 + page);         /* Set page */
    OLED_WriteCommand(0x00 + (col & 0x0F)); /* Set column lower nibble */
    OLED_WriteCommand(0x10 + (col >> 4));   /* Set column upper nibble */
}

/**
 * OLED_Clear - Clear entire display
 */
void OLED_Clear(void)
{
    uint8_t page, col;
    
    for (page = 0; page < OLED_PAGES; page++) {
        for (col = 0; col < OLED_WIDTH; col++) {
            oled_buffer[page][col] = 0x00;
        }
    }
    OLED_Refresh();
}

/**
 * OLED_DisplayON - Turn on display
 */
void OLED_DisplayON(void)
{
    OLED_WriteCommand(0xAF);
}

/**
 * OLED_DisplayOFF - Turn off display
 */
void OLED_DisplayOFF(void)
{
    OLED_WriteCommand(0xAE);
}

/**
 * OLED_SetContrast - Set display contrast
 * contrast: 0-255
 */
void OLED_SetContrast(uint8_t contrast)
{
    OLED_WriteCommand(0x81);
    OLED_WriteCommand(contrast);
}

/**
 * OLED_DrawPixel - Draw pixel at (x, y)
 * color: 1 = white (pixel on), 0 = black (pixel off)
 */
void OLED_DrawPixel(uint16_t x, uint16_t y, uint8_t color)
{
    uint8_t page, bit;
    
    if (x >= OLED_WIDTH || y >= OLED_HEIGHT) {
        return;
    }
    
    page = y / 8;
    bit = y % 8;
    
    if (color) {
        oled_buffer[page][x] |= (1 << bit);
    } else {
        oled_buffer[page][x] &= ~(1 << bit);
    }
}

/**
 * OLED_DrawLine - Draw line using Bresenham algorithm
 */
void OLED_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color)
{
    int dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
    int dy = (y1 > y0) ? (y1 - y0) : (y0 - y1);
    int sx = (x1 > x0) ? 1 : -1;
    int sy = (y1 > y0) ? 1 : -1;
    int err = (dx > dy) ? (dx / 2) : -(dy / 2);
    int x = x0, y = y0;
    
    while (1) {
        OLED_DrawPixel(x, y, color);
        
        if (x == x1 && y == y1) break;
        
        int e2 = err;
        if (e2 > -dx) {
            err -= dy;
            x += sx;
        }
        if (e2 < dy) {
            err += dx;
            y += sy;
        }
    }
}

/**
 * OLED_DrawRect - Draw rectangle
 * fill: 1 = filled, 0 = outline only
 */
void OLED_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t color, uint8_t fill)
{
    uint16_t i, j;
    
    if (fill) {
        for (j = y; j < y + h; j++) {
            for (i = x; i < x + w; i++) {
                OLED_DrawPixel(i, j, color);
            }
        }
    } else {
        for (i = x; i < x + w; i++) {
            OLED_DrawPixel(i, y, color);
            OLED_DrawPixel(i, y + h - 1, color);
        }
        for (j = y; j < y + h; j++) {
            OLED_DrawPixel(x, j, color);
            OLED_DrawPixel(x + w - 1, j, color);
        }
    }
}

/**
 * OLED_DrawChar - Draw single character at (x, y)
 */
void OLED_DrawChar(uint16_t x, uint16_t y, char ch, uint8_t color)
{
    uint8_t i, j, byte;
    uint8_t char_idx;
    
    if (ch < 32 || ch > 126) {
        return;
    }
    
    char_idx = ch - 32;
    
    for (i = 0; i < 5; i++) {
        byte = font_5x8[char_idx][i];
        for (j = 0; j < 8; j++) {
            if (byte & (1 << j)) {
                OLED_DrawPixel(x + i, y + j, color);
            } else {
                OLED_DrawPixel(x + i, y + j, 0);
            }
        }
    }
}

/**
 * OLED_DrawString - Draw string at (x, y)
 */
void OLED_DrawString(uint16_t x, uint16_t y, const char* str, uint8_t color)
{
    uint16_t px = x;
    
    while (*str) {
        OLED_DrawChar(px, y, *str, color);
        px += 6;
        str++;
    }
}

/**
 * OLED_Refresh - Update display with buffer contents
 */
void OLED_Refresh(void)
{
    uint8_t page, col;
    
    for (page = 0; page < OLED_PAGES; page++) {
        OLED_SetPosition(page, 0);
        
        GPIO_ResetPin(GPIOA, 2);    /* CS = 0 */
        GPIO_SetPin(GPIOA, 3);      /* DC = 1 (data mode) */
        
        for (col = 0; col < OLED_WIDTH; col++) {
            SPI_Write(oled_buffer[page][col]);
        }
        
        GPIO_SetPin(GPIOA, 2);      /* CS = 1 */
    }
}
