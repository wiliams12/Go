#include"ui.h"
#include"hash.h"
#include"game_loops.h"

int main(void) {
    Board *board = create_board();
    init_zobrist(board);
    init_hash(board);
    set_up();
    two_players(board);
    free(board);
    return 0;
}