#include"common.h"

#define BOARD_SIZE 19

bool is_move_valid(int **board, Player player, Pos pos);
int place_stone(int **board, Player player, Pos pos);
int **create_board(void);
bool is_captured(int **board, int **captured, Player player);
int remove_from_board(int **board, int **captured);