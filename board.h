#include "common.h"

#define BOARD_SIZE 9

bool is_move_valid(Board *board, Player *player, Pos pos);
int place_stone(Board *board, Player *player, Pos pos);
Board *create_board(void);
int captures(Board *board, Pos move, Player player);
int remove_from_board(Board *board, Group *captured);
bool compare_boards(Board *board_1, Board *board_2);

// Group functions
void initGroup(Group *group);
void addStoneToGroup(Board *board, Group *group, Pos pos); // now handles liberties
void mergeGroups(Board *board, Group *dest, Group *src);    // updates board and liberties

// Liberties functions
int getLiberties(Board *board, Pos pos, Pos **out_liberties);
int removeDuplicateLiberties(Pos *liberties, int count);
bool libertyExists(Pos *liberties, int count, Pos pos);

bool mergeWithAdjacentGroups(Board *board, Pos move, int player_color);

int remove_liberty(Group *group, Pos to_remove);
int update_liberties(Board *board, Group *captured);

bool is_suicide(Board *board, Player *player, Pos pos);
bool is_repetition(Board *board, int player, Pos move);

void score_board(Board *board, float *black_score, float *white_score);
void flood_fill(Board *board, int x, int y, bool **visited, bool *touch_black, bool *touch_white, int *region_size);

bool is_over(Board *board, Player *players);