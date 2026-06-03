#ifndef __TETRIS_H
#define __TETRIS_H

#include <stdint.h>

/* Game Configuration */
#define GAME_WIDTH      10
#define GAME_HEIGHT     20
#define TETROMINO_SIZE  4

/* Game Colors */
#define COLOR_BLACK     0
#define COLOR_WHITE     1

/* Tetromino Types */
#define TETROMINO_I     0
#define TETROMINO_O     1
#define TETROMINO_T     2
#define TETROMINO_S     3
#define TETROMINO_Z     4
#define TETROMINO_J     5
#define TETROMINO_L     6

/* Game States */
#define GAME_STATE_MENU         0
#define GAME_STATE_PLAYING      1
#define GAME_STATE_GAMEOVER     2
#define GAME_STATE_PAUSED       3

/* Tetromino Structures */
typedef struct {
    int8_t x;
    int8_t y;
    uint8_t type;
    uint8_t rotation;
} Tetromino;

typedef struct {
    uint8_t grid[GAME_HEIGHT][GAME_WIDTH];
    uint32_t score;
    uint16_t lines_cleared;
    uint8_t level;
    uint8_t game_state;
    Tetromino current;
    Tetromino next;
    uint32_t drop_time;
    uint32_t last_drop;
} GameState;

/* Function prototypes */
void Tetris_Init(void);
void Tetris_Update(void);
void Tetris_Draw(void);
void Tetris_MoveLeft(void);
void Tetris_MoveRight(void);
void Tetris_MoveDown(void);
void Tetris_Rotate(void);
void Tetris_HardDrop(void);
void Tetris_Pause(void);
void Tetris_Resume(void);
void Tetris_GameOver(void);
uint8_t Tetris_IsGameOver(void);
uint32_t Tetris_GetScore(void);
uint16_t Tetris_GetLines(void);
uint8_t Tetris_GetLevel(void);
const uint8_t* Get_Tetromino_Shape(uint8_t type, uint8_t rotation);
uint8_t Tetris_CheckCollision(int8_t x, int8_t y, uint8_t type, uint8_t rotation);

/* External game state */
extern GameState game;

#endif
