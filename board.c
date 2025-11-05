#include"board.h"

// Creates and returns an array of zeros
int **create_board(void) {
    int **arr = calloc(BOARD_SIZE, sizeof(int *));
    for (int i = 0; i < BOARD_SIZE; i++) {
        arr[i] = calloc(BOARD_SIZE, sizeof(int));
    }
    return arr;
}

int **copy_board(int **board) {
    int **copy = calloc(BOARD_SIZE, sizeof(int *));
    for (int i = 0; i < BOARD_SIZE; i++) {
        copy[i] = calloc(BOARD_SIZE, sizeof(int));
        for (int j = 0; j < BOARD_SIZE; j++) {
            copy[i][j] = board[i][j];
        }
    }

}

// places a move on the board if the move is valid
// players are represented by a number:
// black: 1
// white: 2
int place_stone(int **previous_board, Pos *previous_move, int **board, Player player, Pos pos, Player previous_player) {
    if (is_move_valid(previous_board, board, player, pos, previous_player)) {
        if (previous_move == NULL) {
            previous_move = &pos;
        } else {
            if (place_stone(NULL, NULL, previous_board, previous_player, *previous_move, player) == 1) {
                printf("internal error: mismatch in board history\n");
                return 1;
            }
        }
        board[pos.y][pos.x] = player.num;
        return 0;
    }
    return 1;
}

bool is_move_valid(int **previous_board, int **board, Player player, Pos pos, Player previous_player) {
    if (!((pos.x >= 0) && (pos.x < BOARD_SIZE) && (pos.y >= 0) && (pos.y < BOARD_SIZE))) {
        return false;
    }
    if (board[pos.y][pos.x] != 0) {
        return false;
    }
    
    int **future_board = copy_board(board);
    if (future_board == NULL) {
        printf("Run out of memory\n");
        return false;
    }
    place_stone(NULL, NULL, future_board, player, pos, previous_player);

    if (compare_boards(previous_board, future_board)) {
        free(future_board);
        return false;
    }
    free(future_board);
    return true;
}

bool is_captured(int **board, int **captured, Player player) {

}

// returns the number of captured stones
int remove_from_board(int **board, int **captured) {
    int count = 0;
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
        if (captured[i / BOARD_SIZE][i % BOARD_SIZE] == 1) {
            board[i / BOARD_SIZE][i % BOARD_SIZE] = 0;
            count++;
        }
    }
    return count;
}

bool compare_boards(int **board_1, int **board_2) {
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
        if (board_1[i / BOARD_SIZE][i % BOARD_SIZE] != board_2[i / BOARD_SIZE][i % BOARD_SIZE]) {
            return false;
        }
    }
    return true;
}