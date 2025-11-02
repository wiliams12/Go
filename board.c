#include"board.h"

// Creates and returns an array of zeros
int **create_board(void) {
    int **arr = calloc(BOARD_SIZE, sizeof(int *));
    for (int i = 0; i < BOARD_SIZE; i++) {
        arr[i] = calloc(BOARD_SIZE, sizeof(int));
    }
    return arr;
}

// places a move on the board if the move is valid
// players are represented by a number:
// black: 1
// white: 2
int place_stone(int **board, int player, Pos pos) {
    if (is_move_valid(board, player, pos)) {
        board[pos.y][pos.x];
        return 0;
    }
    return 1;
}

bool is_move_valid(int **board, int player, Pos pos) {
    return true;
}