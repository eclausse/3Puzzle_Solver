#ifndef __TREE_H_
#define __TREE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>

// Forward declaration to solve circular dependency
typedef struct game_t game_t;
typedef enum distance_strategy_e distance_strategy_e;

/* Define g score for first game */
#define ROOT_G_SCORE 0

typedef struct tree_t tree_t;
struct tree_t
{
    tree_t *child[4];

    tree_t * parent; 

    game_t * game;
    uint16_t g, h, f;

    uint8_t visited;
};

/* Return initialise tree_t */
tree_t * init_tree();

/* Return tree_t from a game */
tree_t * create_root(const game_t, const distance_strategy_e);

/* Populate childrens tree_t node */
void populate_tree(tree_t *, const distance_strategy_e);

/* Display a single tree_t */
void display_single_tree_t(const tree_t * const );

/* Display the entiere tree_t */
void display_tree_t(const tree_t * const);

/* Display path from root to the given tree  */
void display_path_tree_t(const tree_t * const);

/* Return tree_t with min f score and not visited or NULL */
tree_t *search_min_f(tree_t *);

/* Free tree allocation must be called after init_tree, create_root, populate_tree*/
void free_tree_t(tree_t *);

#endif