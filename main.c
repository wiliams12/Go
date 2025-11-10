#include"ui.h"
#include"game_loops.h"

int main(void) {
    Board *board = create_board();
    set_up();
    two_players(board);
    free(board);
    return 0;
}