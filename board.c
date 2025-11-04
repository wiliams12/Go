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
int place_stone(int **board, Player player, Pos pos) {
    if (is_move_valid(board, player, pos)) {
        board[pos.y][pos.x] = player.num;
        return 0;
    }
    return 1;
}

bool is_move_valid(int **board, Player player, Pos pos) {
    if (!((pos.x >= 0) && (pos.x < BOARD_SIZE) && (pos.y >= 0) && (pos.y < BOARD_SIZE))) {
        return false;
    }
    if (board[pos.y][pos.x] != 0) {
        return false;
    }
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