#include "stm32f10x.h"
#include "oled.h"
#include "tetris.h"
#include "graphics.h"
#include "clock.h"

/**
 * Graphics_DrawGrid - Draw the game grid
 */
void Graphics_DrawGrid(void)
{
    uint8_t i, j;
    uint16_t x, y;
    
    /* Draw grid lines */
    for (i = 0; i <= GAME_WIDTH; i++) {
        x = GAME_AREA_X + i * BLOCK_SIZE;
        OLED_DrawLine(x, GAME_AREA_Y, x, GAME_AREA_Y + GAME_AREA_H, 1);
    }
    
    for (j = 0; j <= GAME_HEIGHT; j++) {
        y = GAME_AREA_Y + j * BLOCK_SIZE;
        OLED_DrawLine(GAME_AREA_X, y, GAME_AREA_X + GAME_AREA_W, y, 1);
    }
}

/**
 * Graphics_DrawBlock - Draw a single block at grid position
 */
void Graphics_DrawBlock(uint8_t col, uint8_t row, uint8_t type)
{
    uint16_t x = GAME_AREA_X + 1 + col * BLOCK_SIZE;
    uint16_t y = GAME_AREA_Y + 1 + row * BLOCK_SIZE;
    
    if (type > 0) {
        OLED_DrawRect(x, y, BLOCK_SIZE - 1, BLOCK_SIZE - 1, 1, 1);
    }
}

/**
 * Graphics_ClearBlock - Clear a single block
 */
void Graphics_ClearBlock(uint8_t col, uint8_t row)
{
    uint16_t x = GAME_AREA_X + 1 + col * BLOCK_SIZE;
    uint16_t y = GAME_AREA_Y + 1 + row * BLOCK_SIZE;
    
    OLED_DrawRect(x, y, BLOCK_SIZE - 1, BLOCK_SIZE - 1, 0, 1);
}

/**
 * Graphics_DrawTetromino - Draw current falling tetromino
 */
void Graphics_DrawTetromino(int8_t x, int8_t y, uint8_t type, uint8_t rotation)
{
    const uint8_t* shape;
    uint8_t i, j;
    uint16_t px, py;
    
    if (type >= 7) return;
    
    shape = Get_Tetromino_Shape(type, rotation);
    
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (shape[i * 4 + j]) {
                int16_t grid_x = x + j;
                int16_t grid_y = y + i;
                
                if (grid_x >= 0 && grid_x < GAME_WIDTH && 
                    grid_y >= 0 && grid_y < GAME_HEIGHT) {
                    px = GAME_AREA_X + 1 + grid_x * BLOCK_SIZE;
                    py = GAME_AREA_Y + 1 + grid_y * BLOCK_SIZE;
                    OLED_DrawRect(px, py, BLOCK_SIZE - 1, BLOCK_SIZE - 1, 1, 1);
                }
            }
        }
    }
}

/**
 * Graphics_RefreshDisplay - Refresh entire game display
 */
void Graphics_RefreshDisplay(void)
{
    uint8_t i, j;
    
    /* Clear buffer */
    OLED_Clear();
    
    /* Draw grid */
    Graphics_DrawGrid();
    
    /* Draw placed blocks */
    for (i = 0; i < GAME_HEIGHT; i++) {
        for (j = 0; j < GAME_WIDTH; j++) {
            if (game.grid[i][j]) {
                Graphics_DrawBlock(j, i, game.grid[i][j]);
            }
        }
    }
    
    /* Draw current falling piece */
    Graphics_DrawTetromino(game.current.x, game.current.y, 
                           game.current.type, game.current.rotation);
    
    /* Draw info panel */
    Graphics_DrawScore();
    Graphics_DrawNextPiece();
    
    /* Refresh display */
    OLED_Refresh();
}

/**
 * Graphics_DrawScore - Draw score and level info
 */
void Graphics_DrawScore(void)
{
    char buf[16];
    
    /* Draw score label and value */
    OLED_DrawString(INFO_AREA_X, INFO_AREA_Y + 0, "SCORE:", 1);
    
    /* Convert score to string */
    uint32_t score = game.score;
    uint8_t digits[6];
    uint8_t i, digit_count = 0;
    
    if (score == 0) {
        OLED_DrawString(INFO_AREA_X, INFO_AREA_Y + 8, "0", 1);
    } else {
        while (score > 0 && digit_count < 6) {
            digits[digit_count++] = score % 10;
            score /= 10;
        }
        
        for (i = digit_count; i > 0; i--) {
            buf[digit_count - i] = '0' + digits[i - 1];
        }
        buf[digit_count] = '\0';
        OLED_DrawString(INFO_AREA_X, INFO_AREA_Y + 8, buf, 1);
    }
    
    /* Draw level */
    OLED_DrawString(INFO_AREA_X, INFO_AREA_Y + 16, "LEVEL:", 1);
    buf[0] = '0' + game.level;
    buf[1] = '\0';
    OLED_DrawString(INFO_AREA_X, INFO_AREA_Y + 24, buf, 1);
    
    /* Draw lines */
    OLED_DrawString(INFO_AREA_X, INFO_AREA_Y + 32, "LINES:", 1);
    uint16_t lines = game.lines_cleared;
    uint8_t line_digits[4];
    uint8_t line_count = 0;
    
    if (lines == 0) {
        buf[0] = '0';
        buf[1] = '\0';
    } else {
        while (lines > 0 && line_count < 4) {
            line_digits[line_count++] = lines % 10;
            lines /= 10;
        }
        
        for (i = line_count; i > 0; i--) {
            buf[line_count - i] = '0' + line_digits[i - 1];
        }
        buf[line_count] = '\0';
    }
    OLED_DrawString(INFO_AREA_X, INFO_AREA_Y + 40, buf, 1);
}

/**
 * Graphics_DrawNextPiece - Draw preview of next piece
 */
void Graphics_DrawNextPiece(void)
{
    OLED_DrawString(INFO_AREA_X, INFO_AREA_Y + 48, "NEXT:", 1);
    
    const uint8_t* shape = Get_Tetromino_Shape(game.next.type, 0);
    uint8_t i, j;
    uint16_t px, py;
    
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (shape[i * 4 + j]) {
                px = INFO_AREA_X + 1 + j * 3;
                py = INFO_AREA_Y + 56 + i * 3;
                OLED_DrawRect(px, py, 2, 2, 1, 1);
            }
        }
    }
}

/**
 * Graphics_DrawGameOver - Draw game over screen
 */
void Graphics_DrawGameOver(void)
{
    OLED_Clear();
    OLED_DrawString(40, 24, "GAME OVER", 1);
    OLED_DrawString(35, 40, "Score:", 1);
    
    char buf[8];
    uint32_t score = game.score;
    uint8_t i, digit_count = 0;
    uint8_t digits[6];
    
    if (score == 0) {
        buf[0] = '0';
        buf[1] = '\0';
    } else {
        while (score > 0 && digit_count < 6) {
            digits[digit_count++] = score % 10;
            score /= 10;
        }
        
        for (i = digit_count; i > 0; i--) {
            buf[digit_count - i] = '0' + digits[i - 1];
        }
        buf[digit_count] = '\0';
    }
    
    OLED_DrawString(75, 40, buf, 1);
    OLED_Refresh();
}

/**
 * Graphics_DrawMenu - Draw main menu
 */
void Graphics_DrawMenu(void)
{
    OLED_Clear();
    OLED_DrawString(35, 16, "TETRIS GAME", 1);
    OLED_DrawString(20, 32, "Press Start", 1);
    OLED_DrawString(15, 48, "to Begin", 1);
    OLED_Refresh();
}

/**
 * Graphics_Init - Initialize graphics
 */
void Graphics_Init(void)
{
    OLED_Init();
    Graphics_DrawMenu();
}
