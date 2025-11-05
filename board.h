#include"common.h"

#define BOARD_SIZE 19

bool is_move_valid(int **previous_board, int **board, Player player, Pos pos, Player previous_player);
int place_stone(int **previous_board, Pos *previous_move, int **board, Player player, Pos pos, Player previous_player);
int **create_board(void);
bool is_captured(int **board, int **captured, Player player);
int remove_from_board(int **board, int **captured);
bool compare_boards(int **board_1, int **board_2);
int **copy_board(int **board);
