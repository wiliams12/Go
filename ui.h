#include"board.h"


Pos get_move(void);

int set_up(void);
int draw_board(Board *board);
int draw_stone(Board *board, Pos pos);
void DrawTextCentered(const char *text, Rectangle rect, int fontSize, Color color);
void draw_hoshi(int board_size, float tile_size);
