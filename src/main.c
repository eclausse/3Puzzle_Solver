#include <time.h>
#include "taquin.h"

distance_strategy_e menu() {
    char buffer[256];
    memset(buffer, 0,sizeof buffer);
    int i;
    printf("3-Puzzle solver\n");
    printf("Choose a distance: \n");
    do {
        printf("1. Manhattan\n");
        printf("2. Hamming\n");
    } while(!fgets(buffer, sizeof(buffer), stdin) || sscanf(buffer, "%d", &i) != 1 || !(i == 1 || i == 2));
    return i == 1 ? MANHATTAN : HAMMING;
}

int main(char** args, int argc) {
    srand(time(NULL));
    
    game_t g = init_game();
    randomize_game(&g);

    /* Get distance choice from user input */
    distance_strategy_e distance_choice = menu();

    solve(&g, distance_choice);

    return 0;
}