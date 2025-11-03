#include"common.h"

#define BOARD_SIZE 19

bool is_move_valid(int **board, int player, Pos pos);
int place_stone(int **board, int player, Pos pos);
int **create_board(void);