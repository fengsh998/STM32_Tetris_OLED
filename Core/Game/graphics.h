#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <stdint.h>

/* Display Layout Constants */
#define GAME_AREA_X     10          /* Game area start X */
#define GAME_AREA_Y     0           /* Game area start Y */
#define BLOCK_SIZE      5           /* Each tetromino block is 5x5 pixels */
#define GAME_AREA_W     (BLOCK_SIZE * 10 + 1)  /* Game area width */
#define GAME_AREA_H     (BLOCK_SIZE * 20 + 1)  /* Game area height */

#define INFO_AREA_X     65          /* Info display start X */
#define INFO_AREA_Y     0           /* Info display start Y */

/* Color definitions */
#define BLOCK_EMPTY     0
#define BLOCK_I          1
#define BLOCK_O          2
#define BLOCK_T          3
#define BLOCK_S          4
#define BLOCK_Z          5
#define BLOCK_J          6
#define BLOCK_L          7

/* Graphics Functions */
void Graphics_Init(void);
void Graphics_DrawGrid(void);
void Graphics_DrawBlock(uint8_t col, uint8_t row, uint8_t type);
void Graphics_ClearBlock(uint8_t col, uint8_t row);
void Graphics_DrawTetromino(int8_t x, int8_t y, uint8_t type, uint8_t rotation);
void Graphics_DrawNextPiece(void);
void Graphics_DrawScore(void);
void Graphics_DrawGameOver(void);
void Graphics_DrawMenu(void);
void Graphics_RefreshDisplay(void);

#endif
