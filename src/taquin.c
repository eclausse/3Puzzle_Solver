#include "taquin.h"

/* Map to get position from value */
static const position_t solved_game[9] = {
    {1, 1}, {1, 2}, {1, 3},
    {2, 1}, {2, 2}, {2, 3},
    {3, 1}, {3, 2}, {3, 3}
};

game_t init_game() {
    game_t g;
    uint8_t count = 1;
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            g.node[i][j] = count++;
        }
    }
    g.node[2][2] = 0;

    return g;
}

void display_game(const game_t g) {
    printf("\n-------------\n");
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            if(g.node[i][j] != 0) printf("| %d ", g.node[i][j]);
            else printf("|   ");
        }
        printf("|\n-------------\n");
    }
}

position_t get_empty_position(const game_t g) {
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            /* Empty found return it's position*/
            if (!g.node[i][j]) return (position_t){++i, ++j};
        }
    }
    /* Empty not found return error */
    fprintf(stderr, "[Error] %s:%d: Empty node not found", __FILE__, __LINE__);
    exit(1);
}

static position_t get_valid_random_swap_node(const position_t position_empty) {
    uint8_t selected_direction;
    while (1)
    {
        /* Get a number in [0, 3]*/
        selected_direction = rand() % 4;

        if(selected_direction == UP && position_empty.x != 1) {
            return (position_t){position_empty.x - 1, position_empty.y};
        }

        if(selected_direction == DOWN && position_empty.x != 3) {
            return (position_t){position_empty.x + 1, position_empty.y};
        }

        if(selected_direction == LEFT && position_empty.y != 1) {
            return (position_t){position_empty.x, position_empty.y - 1};
        }

        if(selected_direction == RIGHT && position_empty.y != 3) {
            return (position_t){position_empty.x, position_empty.y + 1};
        }
    }
}

void swap_position(game_t * g, position_t p1, position_t p2) {
    assert(p1.x > 0 && p2.x > 0 && p1.y > 0 && p2.y > 0);
    assert(p1.x < 4 && p2.x < 4 && p1.y < 4 && p2.y < 4);

    uint8_t swap;
    /* Swap both nodes */
    swap = g->node[p1.x - 1][p1.y - 1];
    g->node[p1.x - 1][p1.y - 1] = g->node[p2.x - 1][p2.y - 1];
    g->node[p2.x - 1][p2.y - 1] = swap;
}

void randomize_game(game_t * g) {
    assert(g);

    position_t position_empty = get_empty_position(*g);
    position_t position_to_swap;
    
    for (size_t i = 0; i < SHUFFLE; i++)
    {
        /* Get a valid node to swap next to empty node */
        position_to_swap = get_valid_random_swap_node(position_empty);

        swap_position(g, position_empty, position_to_swap);

        /* Update swaped empty position*/
        position_empty = position_to_swap;
    }
}

uint8_t is_same_game(const game_t g1, const game_t g2) {
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            if (g1.node[i][j] != g2.node[i][j]) return 0;
        }
    }
    return 1;
}

uint8_t is_solved_game(const game_t g){
    return heristic(g, MANHATTAN) == 0;
}

position_t get_real_position(const uint8_t number) {
    assert(number < 9 && number > -1);
    if (number == 0) return solved_game[8];
    return solved_game[number - 1];
}

static uint8_t manhattan(const position_t a, const position_t b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

static uint8_t hamming(const uint8_t number, const position_t a) {
    position_t r = get_real_position(number);
    return !(r.x == a.x && r.y == a.y);
}

static uint8_t heristic_manhattan(const game_t g) {
    uint8_t score_total = 0, value, score_temp;
    position_t wanted_position;

    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            value = g.node[i][j];
            wanted_position = get_real_position(value);
            score_temp = manhattan((position_t){i+1, j+1}, wanted_position);
            score_total += score_temp;
        }
    }
    return score_total;
}

static uint8_t heristic_hamming(const game_t g) {
    uint8_t score_total = 0;

    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            score_total += hamming(g.node[i][j], (position_t){i+1,j+1});;
        }
    }
    return score_total;
}

uint8_t heristic(const game_t g, const distance_strategy_e distance_strategy) {
    if(distance_strategy == MANHATTAN) return heristic_manhattan(g);
    return heristic_hamming(g);
}

uint8_t solve(game_t * g, const distance_strategy_e distance_strategy) {
    assert(g);
    uint16_t explored_possibilities = 0;
    int tmp;

    /* Create possiblibilites tree_t */
    tree_t * possibilities = create_root(*g, distance_strategy);
    tree_t * best_possibility = NULL;

    while (best_possibility = search_min_f(possibilities))
    {
        if (is_solved_game(* best_possibility->game)) break;
        populate_tree(best_possibility, distance_strategy);
        best_possibility->visited = 1;
        explored_possibilities++;
    }

    /* Show entire tree */
    //display_tree_t(possibilities);

    /* Display solution */
    printf("\n\nSolution found ! \nExplored possibilities: %d\n", explored_possibilities);
    display_path_tree_t(best_possibility);

    free_tree_t(possibilities);
    
    return explored_possibilities;
}