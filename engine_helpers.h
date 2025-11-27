#include"board.h"
#include <string.h>

int get_legal_moves(Board *board, Pos *moves,  Player *player);
bool play_move(Board *board, Pos move, Player *players);
bool undo_move(Board *board);
void undo_capture(Board *board, Group *captured);

Board *copy_board(Board *src);