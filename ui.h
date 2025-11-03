#include"board.h"
#include<raylib.h>


Pos get_move(int **board);

int set_up(void);
int draw_board(int **board, int player);
int draw_stone(int player, Pos pos);

