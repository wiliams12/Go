#include"engine_helpers.h"
#include"hash.h"


int get_legal_moves(Board *board, Pos *moves, Player *player) {
    int found = 0;
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
        Pos current = {i / BOARD_SIZE, i % BOARD_SIZE};
        if (board->board[current.y][current.x] == 0) {
            if (is_move_valid(board, player, current)) {
                if (moves != NULL) {
                    moves[found++] = (Pos){i / BOARD_SIZE, i % BOARD_SIZE};
                } else {
                    found++;
                }
            }
        }
    }
    return found;
}

bool play_move(Board *board, Pos move, Player *players) {
    if (place_stone(board, &players[board->move_num % 2], move) == 0) {
        if (!mergeWithAdjacentGroups(board, move, players[board->move_num % 2].num)) {
            Group *group = malloc(sizeof(Group));
            initGroup(group);
            board->groups[move.y][move.x] = group;
            addStoneToGroup(board, group, move);
        }
        update_hash(board, move, players[board->move_num % 2].num);
        update_board_history(board, move);

        players[board->move_num % 2 == 1].score += captures(board, move);
        board->move_num++;
        return true;
    } else {
        return false;
    }
}

bool undo_move(Board *board) {
    // ! issues with switching players
    if (board->move_num <= 0) {
        return false;
    }
        
    if (board->move_num < 2) {
        board->hash = 0;
    }
    board->move_num--;
    
    Pos last = board->moves[board->move_num];
    int last_color = board->board[last.y][last.x];
    update_hash(board, last, last_color);
    board->board[last.y][last.x] = 0;
    board->groups[last.y][last.x] = NULL;
    
    Pos dirs[4] = {
        { last.x + 1, last.y },
        { last.x - 1, last.y },
        { last.x, last.y + 1 },
        { last.x, last.y - 1 }
    };

    for (int d = 0; d < 4; d++) {
        int x = dirs[d].x;
        int y = dirs[d].y;

        if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE)
            continue;

        if (board->groups[y][x] == NULL) {
            continue;
        }

        Group *group = board->groups[y][x];
        if (group->liberty_count >= group->liberty_capacity) {
            int new_cap = group->liberty_capacity * 2;
            Pos *new_arr = realloc(group->liberties, new_cap * sizeof(Pos));
            if (!new_arr) {
                printf("Memory error: Couldn't allocate memory.\n");
                continue;
            }
            group->liberties = new_arr;
            group->liberty_capacity = new_cap;
        }
        group->liberties[group->liberty_count++] = (Pos){ last.x, last.y };
    }

    Group *cap = board->captured[board->move_num]; // ! this is shady
    if (cap != NULL) {
        // restore all stones in the captured group
        undo_capture(board, cap);
    }


    if (cap != NULL) {
        free(cap->stones);
        free(cap->liberties);
        free(cap);
        board->captured[board->move_num] = NULL;
    }

    return true;
}

void undo_capture(Board *board, Group *captured) {
    if (captured == NULL) return;

    int restored_color = (board->move_num % 2 == 1) ? 2 : 1;


    for (int i = 0; i < captured->size; i++) {

        Pos pos = captured->stones[i];

        board->board[pos.y][pos.x] = restored_color;
        board->groups[pos.y][pos.x] = captured;

    }

    // Restore liberties: the only liberty is the last move that captured this group
    Pos last_move = board->moves[board->move_num - 1];
    captured->liberty_count = 1;
    captured->liberties[0] = last_move;
}

