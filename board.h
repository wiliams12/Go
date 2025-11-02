#include"common.h"

#define BOARD_SIZE 19

bool is_move_valid(int **board, int player, int pos[2]);
int place_stone(int **board, int player, int pos[2]);
int **create_board(void);