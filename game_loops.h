#include"board.h"
#include"ui.h"
#include"engine_helpers.h"
#include"monte_carlo.h"

int two_players(Board *board);
void test_undo(Board *board);
int computer(Board *board);
Pos mcts_get_best_move(Board *board, Node *root_state, Player *players, int iterations);
