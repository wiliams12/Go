#include"ui.h"
#include"game_loops.h"

int main(void) {
    int **board = create_board();
    set_up();
    two_players(board);
    return 0;
}