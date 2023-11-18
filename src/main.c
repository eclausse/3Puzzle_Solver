#include <time.h>
#include "taquin.h"

int main(char** args, int argc) {
    srand( time( NULL ) );
    
    game_t g = init_game();
    randomize_game(&g);

    solve(&g, MANHATTAN);

    return 0;
}