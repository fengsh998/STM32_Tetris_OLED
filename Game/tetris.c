#include "stm32f10x.h"
#include "tetris.h"
#include "oled.h"
#include "clock.h"

/* Game state instance */
GameState game;

/* Tetromino shapes (rotation states) */
/* I-Tetromino (4x4) */
const uint8_t tetromino_I[4][16] = {
    {0,0,0,0, 1,1,1,1, 0,0,0,0, 0,0,0,0},
    {0,0,1,0, 0,0,1,0, 0,0,1,0, 0,0,1,0},
    {0,0,0,0, 0,0,0,0, 1,1,1,1, 0,0,0,0},
    {0,1,0,0, 0,1,0,0, 0,1,0,0, 0,1,0,0}
};

/* O-Tetromino (2x2) - no rotation needed */
const uint8_t tetromino_O[4][16] = {
    {1,1,0,0, 1,1,0,0, 0,0,0,0, 0,0,0,0},
    {1,1,0,0, 1,1,0,0, 0,0,0,0, 0,0,0,0},
    {1,1,0,0, 1,1,0,0, 0,0,0,0, 0,0,0,0},
    {1,1,0,0, 1,1,0,0, 0,0,0,0, 0,0,0,0}
};

/* T-Tetromino */
const uint8_t tetromino_T[4][16] = {
    {0,1,0,0, 1,1,1,0, 0,0,0,0, 0,0,0,0},
    {0,1,0,0, 0,1,1,0, 0,1,0,0, 0,0,0,0},
    {0,0,0,0, 1,1,1,0, 0,1,0,0, 0,0,0,0},
    {0,1,0,0, 1,1,0,0, 0,1,0,0, 0,0,0,0}
};

/* S-Tetromino */
const uint8_t tetromino_S[4][16] = {
    {0,1,1,0, 1,1,0,0, 0,0,0,0, 0,0,0,0},
    {0,1,0,0, 0,1,1,0, 0,0,1,0, 0,0,0,0},
    {0,0,0,0, 0,1,1,0, 1,1,0,0, 0,0,0,0},
    {0,0,1,0, 0,1,1,0, 0,1,0,0, 0,0,0,0}
};

/* Z-Tetromino */
const uint8_t tetromino_Z[4][16] = {
    {1,1,0,0, 0,1,1,0, 0,0,0,0, 0,0,0,0},
    {0,0,1,0, 0,1,1,0, 0,1,0,0, 0,0,0,0},
    {0,0,0,0, 1,1,0,0, 0,1,1,0, 0,0,0,0},
    {0,1,0,0, 1,1,0,0, 1,0,0,0, 0,0,0,0}
};

/* J-Tetromino */
const uint8_t tetromino_J[4][16] = {
    {0,1,0,0, 0,1,0,0, 1,1,0,0, 0,0,0,0},
    {1,0,0,0, 1,1,1,0, 0,0,0,0, 0,0,0,0},
    {1,1,0,0, 1,0,0,0, 1,0,0,0, 0,0,0,0},
    {0,0,0,0, 1,1,1,0, 0,0,1,0, 0,0,0,0}
};

/* L-Tetromino */
const uint8_t tetromino_L[4][16] = {
    {1,0,0,0, 1,0,0,0, 1,1,0,0, 0,0,0,0},
    {0,0,0,0, 1,1,1,0, 1,0,0,0, 0,0,0,0},
    {1,1,0,0, 0,1,0,0, 0,1,0,0, 0,0,0,0},
    {0,0,1,0, 1,1,1,0, 0,0,0,0, 0,0,0,0}
};

/**
 * Get tetromino shape for a type
 */
const uint8_t* Get_Tetromino_Shape(uint8_t type, uint8_t rotation)
{
    const uint8_t (*shapes)[4][16] = NULL;
    
    switch (type) {
        case TETROMINO_I: return &tetromino_I[rotation % 4][0];
        case TETROMINO_O: return &tetromino_O[rotation % 4][0];
        case TETROMINO_T: return &tetromino_T[rotation % 4][0];
        case TETROMINO_S: return &tetromino_S[rotation % 4][0];
        case TETROMINO_Z: return &tetromino_Z[rotation % 4][0];
        case TETROMINO_J: return &tetromino_J[rotation % 4][0];
        case TETROMINO_L: return &tetromino_L[rotation % 4][0];
        default: return &tetromino_I[0][0];
    }
}

/**
 * Check collision with grid or boundaries
 */
uint8_t Tetris_CheckCollision(int8_t x, int8_t y, uint8_t type, uint8_t rotation)
{
    const uint8_t* shape = Get_Tetromino_Shape(type, rotation);
    int8_t i, j;
    
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (shape[i * 4 + j]) {
                int8_t grid_x = x + j;
                int8_t grid_y = y + i;
                
                /* Check boundaries */
                if (grid_x < 0 || grid_x >= GAME_WIDTH) {
                    return 1;
                }
                if (grid_y < 0) {
                    continue;
                }
                if (grid_y >= GAME_HEIGHT) {
                    return 1;
                }
                
                /* Check grid collision */
                if (game.grid[grid_y][grid_x]) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

/**
 * Place tetromino on grid
 */
void Tetris_PlaceTetromino(int8_t x, int8_t y, uint8_t type, uint8_t rotation)
{
    const uint8_t* shape = Get_Tetromino_Shape(type, rotation);
    int8_t i, j;
    
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (shape[i * 4 + j]) {
                int8_t grid_x = x + j;
                int8_t grid_y = y + i;
                
                if (grid_x >= 0 && grid_x < GAME_WIDTH && 
                    grid_y >= 0 && grid_y < GAME_HEIGHT) {
                    game.grid[grid_y][grid_x] = type + 1;
                }
            }
        }
    }
}

/**
 * Clear completed lines
 */
void Tetris_ClearLines(void)
{
    uint8_t row, col, full_rows = 0;
    uint8_t i, j, k;
    
    for (row = 0; row < GAME_HEIGHT; row++) {
        uint8_t is_full = 1;
        
        for (col = 0; col < GAME_WIDTH; col++) {
            if (!game.grid[row][col]) {
                is_full = 0;
                break;
            }
        }
        
        if (is_full) {
            full_rows++;
            
            /* Move rows down */
            for (i = row; i > 0; i--) {
                for (j = 0; j < GAME_WIDTH; j++) {
                    game.grid[i][j] = game.grid[i-1][j];
                }
            }
            
            /* Clear top row */
            for (j = 0; j < GAME_WIDTH; j++) {
                game.grid[0][j] = 0;
            }
            
            row--;
        }
    }
    
    /* Update score and lines */
    if (full_rows) {
        game.lines_cleared += full_rows;
        
        /* Scoring: 100 * rows^2 */
        uint32_t points = 100 * full_rows * full_rows;
        game.score += points;
        
        /* Update level every 10 lines */
        game.level = game.lines_cleared / 10 + 1;
        
        /* Increase drop speed */
        if (game.drop_time > 100) {
            game.drop_time -= 50;
        }
    }
}

/**
 * Generate next tetromino
 */
void Tetris_GenerateNext(void)
{
    game.current = game.next;
    game.next.type = Get_Systick() % 7;
    game.next.rotation = 0;
    game.next.x = GAME_WIDTH / 2 - 2;
    game.next.y = 0;
}

/**
 * Tetris_Init - Initialize game
 */
void Tetris_Init(void)
{
    uint8_t i, j;
    
    /* Clear grid */
    for (i = 0; i < GAME_HEIGHT; i++) {
        for (j = 0; j < GAME_WIDTH; j++) {
            game.grid[i][j] = 0;
        }
    }
    
    game.score = 0;
    game.lines_cleared = 0;
    game.level = 1;
    game.game_state = GAME_STATE_PLAYING;
    game.drop_time = 500;           /* Initial drop time: 500ms */
    game.last_drop = Get_Systick();
    
    /* Spawn first tetromino */
    game.current.type = Get_Systick() % 7;
    game.current.rotation = 0;
    game.current.x = GAME_WIDTH / 2 - 2;
    game.current.y = 0;
    
    Tetris_GenerateNext();
    
    /* Check if spawn location is clear */
    if (Tetris_CheckCollision(game.current.x, game.current.y, game.current.type, game.current.rotation)) {
        game.game_state = GAME_STATE_GAMEOVER;
    }
}

/**
 * Tetris_Update - Update game state
 */
void Tetris_Update(void)
{
    uint32_t current_time = Get_Systick();
    
    if (game.game_state != GAME_STATE_PLAYING) {
        return;
    }
    
    /* Check if it's time to drop */
    if ((current_time - game.last_drop) >= game.drop_time) {
        game.last_drop = current_time;
        
        /* Try to move down */
        if (!Tetris_CheckCollision(game.current.x, game.current.y + 1, 
                                   game.current.type, game.current.rotation)) {
            game.current.y++;
        } else {
            /* Place and spawn new piece */
            Tetris_PlaceTetromino(game.current.x, game.current.y, 
                                  game.current.type, game.current.rotation);
            Tetris_ClearLines();
            Tetris_GenerateNext();
            
            /* Check game over */
            if (Tetris_CheckCollision(game.current.x, game.current.y, 
                                      game.current.type, game.current.rotation)) {
                game.game_state = GAME_STATE_GAMEOVER;
            }
        }
    }
}

/**
 * Tetris_MoveLeft - Move current piece left
 */
void Tetris_MoveLeft(void)
{
    if (game.game_state != GAME_STATE_PLAYING) return;
    
    if (!Tetris_CheckCollision(game.current.x - 1, game.current.y, 
                               game.current.type, game.current.rotation)) {
        game.current.x--;
    }
}

/**
 * Tetris_MoveRight - Move current piece right
 */
void Tetris_MoveRight(void)
{
    if (game.game_state != GAME_STATE_PLAYING) return;
    
    if (!Tetris_CheckCollision(game.current.x + 1, game.current.y, 
                               game.current.type, game.current.rotation)) {
        game.current.x++;
    }
}

/**
 * Tetris_MoveDown - Move current piece down
 */
void Tetris_MoveDown(void)
{
    if (game.game_state != GAME_STATE_PLAYING) return;
    
    if (!Tetris_CheckCollision(game.current.x, game.current.y + 1, 
                               game.current.type, game.current.rotation)) {
        game.current.y++;
        game.last_drop = Get_Systick();
    }
}

/**
 * Tetris_Rotate - Rotate current piece
 */
void Tetris_Rotate(void)
{
    uint8_t new_rotation;
    
    if (game.game_state != GAME_STATE_PLAYING) return;
    
    new_rotation = (game.current.rotation + 1) % 4;
    
    if (!Tetris_CheckCollision(game.current.x, game.current.y, 
                               game.current.type, new_rotation)) {
        game.current.rotation = new_rotation;
    }
}

/**
 * Tetris_Pause - Pause game
 */
void Tetris_Pause(void)
{
    if (game.game_state == GAME_STATE_PLAYING) {
        game.game_state = GAME_STATE_PAUSED;
    } else if (game.game_state == GAME_STATE_PAUSED) {
        game.game_state = GAME_STATE_PLAYING;
    }
}

/**
 * Tetris_GameOver - End game
 */
void Tetris_GameOver(void)
{
    game.game_state = GAME_STATE_GAMEOVER;
}

/**
 * Tetris_IsGameOver - Check if game over
 */
uint8_t Tetris_IsGameOver(void)
{
    return (game.game_state == GAME_STATE_GAMEOVER);
}

/**
 * Tetris_GetScore - Get current score
 */
uint32_t Tetris_GetScore(void)
{
    return game.score;
}

/**
 * Tetris_GetLines - Get cleared lines
 */
uint16_t Tetris_GetLines(void)
{
    return game.lines_cleared;
}

/**
 * Tetris_GetLevel - Get current level
 */
uint8_t Tetris_GetLevel(void)
{
    return game.level;
}
