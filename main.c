#include"ui.h"
#include"game_loops.h"

int main(void) {
    int **board = create_board();
    int **captured = create_board();
    int **previous_board = create_board();
    set_up();
    two_players(previous_board, board, captured);
    free(board);
    free(captured);
    free(previous_board);
    return 0;
}