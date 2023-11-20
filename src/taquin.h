#ifndef __TAQUIN_H_
#define __TAQUIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include "tree.h"

#define MAX(X, Y) X < Y ? Y : X

/* Number of swap to get a random game */
#define SHUFFLE 100

#define N 3

typedef struct game_t game_t;

struct game_t
{
    uint8_t node[N][N];
};

/* Coordinates of nodes in [1, 3]*/
typedef struct position_t position_t;
struct position_t
{
    uint8_t x, y;
};

typedef enum direction_e {
    UP, DOWN, RIGHT, LEFT
} direction_e;

typedef enum distance_strategy_e {
    MANHATTAN, HAMMING
} distance_strategy_e;

/* Return initialise game_t */
game_t init_game();

/* Display game_t */
void display_game(const game_t);

/* Require srand initiliased
   Return randomized solveble game_t */
void randomize_game(game_t *);

uint8_t is_same_game(const game_t g1, const game_t g2);

/* Return 1 if game is the solved game or 0 */
uint8_t is_solved_game(const game_t);

/* Return position_t of 0 in game_t */
position_t get_empty_position(const game_t);

/* Get real position in solved game from a value 
i.e 0 => 3, 3; 1 => 1, 1 */
position_t get_real_position(const uint8_t);

/* Swap in game node in p1 and p2 positions */
void swap_position(game_t *, position_t, position_t);

/* Calculate heristic score */
uint8_t heristic(const game_t, const distance_strategy_e distance_strategy);

/* Solve game with A* using distance_strategy as heristic
   Return number swap */
uint8_t solve(game_t *, const distance_strategy_e);

#endif