#include"engine_helpers.h"
#include"hash.h"


int get_legal_moves(Board *board, Pos *moves, Player *player) {
    int found = 0;
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
        Pos current = {i / BOARD_SIZE, i % BOARD_SIZE};
        if (board->board[current.y][current.x]== 0) {
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

        players[board->move_num % 2 == 1].score += captures(board, move, players[board->move_num % 2]);
        board->move_num++;
        return true;
    } else {
        return false;
    }
}

bool undo_move(Board *board) {
    if (board->move_num <= 0)
        return false;

    int idx = board->move_num - 1;

    if (board->move_num >= 2)
        board->hash = board->board_history[board->move_num - 2];
    else
        board->hash = 0;

    Pos last = board->moves[idx];
    int last_color = board->board[last.y][last.x];
    update_hash(board, last, last_color);
    board->board[last.y][last.x] = 0;
    board->groups[last.y][last.x] = NULL;

    Group *cap = board->captured[idx];
    if (cap != NULL) {
        // restore all stones in the captured group
        undo_capture(board, cap);
    } else {
        printf("no capture\n");
    }

    board->move_num--;

    if (cap != NULL) {
        free(cap->stones);
        free(cap->liberties);
        free(cap);
        board->captured[idx] = NULL;
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

