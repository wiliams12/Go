#include"engine_helpers.h"


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

bool play_move(Board *board, Pos move, Player *players, int turn) {
    if (place_stone(board, &players[turn % 2], move) == 0) {
        if (!mergeWithAdjacentGroups(board, move, players[turn % 2].num)) {
            Group *group = malloc(sizeof(Group));
            initGroup(group);
            board->groups[move.y][move.x] = group;
            addStoneToGroup(board, group, move);
        }
        update_hash(board, move, players[turn % 2].num);
        update_board_history(board, move);

        players[(turn + 1) % 2].score += captures(board, move, players[turn % 2]);
        board->move_num++;
        return true;
    } else {
        return false;
    }
}

bool undo_move(Board *board) {
    board->hash = board->board_history[board->move_num-2];
    Pos last_move = board->moves[board->move_num-1];
    board->board[last_move.y][last_move.x] = 0;

    // No need to change the values in the arrays with int etc. because they still take up that space so it is almost useless and complicated
    board->move_num--;
    // Can decrement before because it would be the index move_num-1
    Group *captured = board->captured[board->move_num];

    if (captured != NULL) {
        free(captured->stones);
        free(captured->liberties);
        free(captured);
    }
}

