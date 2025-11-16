#include"board.h"

int get_legal_moves(Board *board, Pos *moves,  Player *player);
bool play_move(Board *board, Pos move, Player *players, int turn);
bool undo_move(Board *board);
void undo_capture(Board *board, Group *captured);