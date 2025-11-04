#include"ui.h"
#include"game_loops.h"

int main(void) {
    int **board = create_board();
    int **captured = create_board();
    set_up();
    two_players(board, captured);
    return 0;
}