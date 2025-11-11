#include"common.h"

int init_zobrist(Board *board);
int init_hash(Board *board);
void update_hash(Board *board, Pos move, int player);

int update_board_history(Board *board);
bool is_repeated_position(Board *board);