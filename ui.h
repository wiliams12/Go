#include"board.h"


Pos get_move(int **board);

int set_up(void);
int draw_board(int **board);
int draw_stone(int current_player, Pos pos);
void DrawTextCentered(const char *text, Rectangle rect, int fontSize, Color color);

