#include"hash.h"
#include"board.h"

int init_zobrist(Board *board) {
    srand(time(NULL));

    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE * 2; i++) {
        uint64_t rand64 = ((uint64_t)rand() << 32) ^ rand();
        board->hash_table[i] = rand64;
    }

    return 0;
}

int init_hash(Board *board) {
    if (board->hash_table == 0 || board->hash != 0) {
        printf("Hash initialization mismatch\n");
        return 0;
    }
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            int color = board->board[y][x];

            if (color == 0)
                continue;

            int index = (y * BOARD_SIZE + x) + (color - 1) * BOARD_SIZE * BOARD_SIZE;
            board->hash ^= board->hash_table[index];
        }
    }

    return 0;
}

void update_hash(Board *board, Pos move, int player) {
    board->hash ^= board->hash_table[move.y * BOARD_SIZE + move.x + (player - 1) * BOARD_SIZE * BOARD_SIZE];
}


int update_board_history(Board *board, Pos move) {
    if (board->move_num >= board->history_capacity) {
        board->history_capacity *= 2;
        board->board_history = realloc(board->board_history, board->history_capacity * sizeof(uint64_t));
        board->moves = realloc(board->moves, board->history_capacity * sizeof(Pos));
        board->captured = realloc(board->captured, board->history_capacity * sizeof(Group));
        if(board->board_history == NULL || board->moves == NULL) {
            printf("Internal error: board history can't be stored\n");
        }
    }
    board->board_history[board->move_num] = board->hash;
    board->moves[board->move_num] = move;
    return 0;
}

// Checks for repetition (Ko / Superko)
bool is_repeated_position(Board *board) {
    for (int i = board->move_num; i >= 0; i--) {
        if (board->board_history[i] == board->hash) {
            return true;
        }
    }
    return false;
}