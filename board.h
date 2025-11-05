#include"common.h"

#define BOARD_SIZE 19

bool is_move_valid(int **board, Player player, Pos pos);
int place_stone(int **board, Player player, Pos pos);
int **create_board(void);
bool is_captured(int **board, int **captured, Player player);
int remove_from_board(int **board, int **captured);
bool compare_boards(int **board_1, int **board_2);
int **copy_board(int **board);
void update_group_array(GroupList **pointer, Pos to_add);

void initGroup(Group *group);
void initGroupList(GroupList *list);

void addStoneToGroup(Group *group, Pos pos);
void addGroup(GroupList *list, Group group);
