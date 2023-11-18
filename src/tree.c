#include "tree.h"
#include "taquin.h"

tree_t * init_tree() {
    tree_t * t = malloc(sizeof(tree_t));
    if (!t) {
        fprintf(stderr, "[Error] %s:%d: Couldn't malloc tree", __FILE__, __LINE__);
        exit(1);
    }

    t->parent = NULL;

    game_t * g = malloc(sizeof(game_t));
    if (!g) {
        fprintf(stderr, "[Error] %s:%d: Couldn't malloc game_t", __FILE__, __LINE__);
        exit(1);
    }
    t->game = g;

    for (size_t i = 0; i < 4; i++)
    {
        t->child[i] = NULL;
    }

    t->g = 0;
    t->h = 0;
    t->f = 0;

    t->visited = 0;

    return t;
}

tree_t * create_tree_part(const game_t game, direction_e direction, tree_t * parent, const distance_strategy_e distance_strategy) {
    position_t position_empty =  get_empty_position(game);

    /* Get swap node position */
    position_t position_to_swap;
    switch (direction)
    {
    case UP:
        position_to_swap = (position_t){position_empty.x - 1, position_empty.y};
        break;
    case DOWN:
        position_to_swap = (position_t){position_empty.x + 1, position_empty.y};
        break;
    case LEFT:
        position_to_swap = (position_t){position_empty.x, position_empty.y - 1};
        break;
    case RIGHT:
        position_to_swap = (position_t){position_empty.x, position_empty.y + 1};
        break;
    default:
        exit(1);
        break;
    }

    /* Swap game node */
    game_t new_game = game;
    swap_position(&new_game, position_empty, position_to_swap);
    /* Ensure that no loop swap are made */
    /* e.g parent swap right and child swap left */
    if (parent && parent->parent && is_same_game(* (parent->parent->game), new_game)) return NULL;

    tree_t * new_tree = init_tree();

    /* Copy modified parent game */
    *(new_tree->game) = new_game;

    /* Link parent tree_t */
    new_tree->parent = parent;

    /* Calculate scores */
    if (!parent) 
        new_tree->g = 1;
    else 
        new_tree->g = parent->g + 1;
    new_tree->h = heristic(*new_tree->game, MANHATTAN);
    new_tree->f = new_tree->g + new_tree->h;

    new_tree->visited = 0;

    return new_tree;
}

tree_t * create_root(const game_t g, const distance_strategy_e distance_strategy) {
    tree_t * root = init_tree();

    *root->game = g;
    root->h = heristic(g, MANHATTAN);
    root->f = root->h;

    position_t position_empty =  get_empty_position(g);
    if (position_empty.x != 1) root->child[0] = create_tree_part(g, UP, root, distance_strategy);
    if (position_empty.x != 3) root->child[1] = create_tree_part(g, DOWN, root, distance_strategy);
    if (position_empty.y != 1) root->child[2] = create_tree_part(g, LEFT, root, distance_strategy);
    if (position_empty.y != 3) root->child[3] = create_tree_part(g, RIGHT, root, distance_strategy);
    
    return root;
}

void populate_tree(tree_t * t, const distance_strategy_e distance_strategy) {
    assert(t);
    assert(t->game);

    game_t game = *t->game;
    position_t position_empty =  get_empty_position(game);
    if (position_empty.x != 1) t->child[0] = create_tree_part(game, UP, t, distance_strategy);
    if (position_empty.x != 3) t->child[1] = create_tree_part(game, DOWN, t, distance_strategy);
    if (position_empty.y != 1) t->child[2] = create_tree_part(game, LEFT, t, distance_strategy);
    if (position_empty.y != 3) t->child[3] = create_tree_part(game, RIGHT, t, distance_strategy);
}

void display_single_tree_t(const tree_t * const t) {
    if (!t) return;
    printf("Tree: ");
    if (t->game) display_game(*t->game);
    printf("Visited: %s\n", t->visited ? "True" : "False");
    printf("Scores: \n");
    printf("G(x) = %d | H(x) = %d | F(x) = %d\n", t->g, t->h, t->f);
}

void display_tree_t(const tree_t * const t) {
    if (!t) return;
    display_single_tree_t(t);
    
    for (size_t i = 0; i < 4; i++)
    {
        if(t->child[i]) display_tree_t(t->child[i]);
    }
}

void free_tree_t(tree_t * t) {
    if (!t) return;
    for (size_t i = 0; i < 4; i++)
    {
        if(t->child[i]) free_tree_t(t->child[i]);
    }
    free(t->game);
    t->game = NULL;
    free(t);
    t = NULL;
}

tree_t *search_min_f(tree_t * t) {
    assert(t);
    tree_t *t1 = NULL; /* Contain child with min f */
    tree_t *t2 = NULL; /* Temporary child */

    for (size_t i = 0; i < 4; i++)
    {
        if(t->child[i]) {
            if(!t1) {
                t1 = search_min_f(t->child[i]);
            } else {
                t2 = search_min_f(t->child[i]);
                if (t2 && !t2->visited) t1 = t1->f < t2->f ? t1 : t2;
            }
        }
    }
    if (t1 && !t1->visited) return  t1->f < t->f ? t1 : !t->visited ? t : t1;
    return !t->visited ? t : NULL;
}