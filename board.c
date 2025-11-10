#include"board.h"

Board *create_board(void) {
    Board *b = malloc(sizeof(Board));

    b->board = calloc(BOARD_SIZE, sizeof(int *));
    for (int i = 0; i < BOARD_SIZE; i++) {
        b->board[i] = calloc(BOARD_SIZE, sizeof(int));
    }

    b->groups = calloc(BOARD_SIZE, sizeof(Group **));
    for (int i = 0; i < BOARD_SIZE; i++) {
        b->groups[i] = calloc(BOARD_SIZE, sizeof(Group *));
    }

    return b;
}


// Group logic ----------- TODO: check if correct

bool mergeWithAdjacentGroups(Board *board, Pos move, int player_color) {
    Group *target_group = NULL;
    int dirs[4][2] = {{0,1},{1,0},{0,-1},{-1,0}}; // up, right, down, left

    for (int i = 0; i < 4; i++) {
        int nx = move.x + dirs[i][0];
        int ny = move.y + dirs[i][1];

        if (nx < 0 || ny < 0 || nx >= BOARD_SIZE || ny >= BOARD_SIZE)
            continue;

        Group *neighbor_group = board->groups[ny][nx];
        if (neighbor_group == NULL) {
            continue;
        }
        // Check color of the group
        if (board->board[ny][nx] != player_color) {
            remove_liberty(board->groups[ny][nx], (Pos){nx,ny});
            continue;
            
        }
        if (target_group == NULL) {
            // First same-color group found → add stone here
            target_group = neighbor_group;
            addStoneToGroup(board, target_group, move);
        } else if (neighbor_group != target_group) {
            // Merge this other adjacent group into the target group
            mergeGroups(board, target_group, neighbor_group);
        }
    }

    return target_group != NULL;
}



void initGroup(Group *group) {
    group->size = 0;
    group->capacity = 4;
    group->stones = malloc(group->capacity * sizeof(Pos));

    group->liberty_count = 0;
    group->liberty_capacity = 4;
    group->liberties = calloc(group->liberty_capacity, sizeof(Pos));
}



void addStoneToGroup(Board *board, Group *group, Pos pos) {
    // Add stone to the group
    if (group->size >= group->capacity) {
        group->capacity *= 2;
        group->stones = realloc(group->stones, group->capacity * sizeof(Pos));
    }
    group->stones[group->size++] = pos;

    // Update the board pointer
    board->groups[pos.y][pos.x] = group;

    // Add new liberties for this stone
    Pos *new_libs;
    int new_lib_count = getLiberties(board, pos, &new_libs);

    for (int i = 0; i < new_lib_count; i++) {
        Pos lib = new_libs[i];

        if (!libertyExists(group->liberties, group->liberty_count, lib)) {
            if (group->liberty_count >= group->liberty_capacity) {
                group->liberty_capacity *= 2;
                group->liberties = realloc(group->liberties, group->liberty_capacity * sizeof(Pos));
            }
            group->liberties[group->liberty_count++] = lib;
        }
    }

    free(new_libs);
}



// Returns true if pos exists in liberties array, false otherwise
bool libertyExists(Pos *liberties, int count, Pos pos) {
    for (int i = 0; i < count; i++) {
        if (liberties[i].x == pos.x && liberties[i].y == pos.y) {
            return true;
        }
    }
    return false;
}


void mergeGroups(Board *board, Group *dest, Group *src) {
    for (int i = 0; i < src->size; i++) {
        Pos pos = src->stones[i];
        addStoneToGroup(board, dest, pos);
        board->groups[pos.y][pos.x] = dest;
    }

    free(src->stones);
    free(src->liberties);
    free(src);
}

// Returns an array of empty neighboring positions of pos
// The caller must free the returned array if needed
int getLiberties(Board *board, Pos pos, Pos **out_liberties) {
    int capacity = 4;
    int count = 0;
    *out_liberties = malloc(capacity * sizeof(Pos));

    int dirs[4][2] = {{0,1},{1,0},{0,-1},{-1,0}}; // up, right, down, left

    for (int i = 0; i < 4; i++) {
        int nx = pos.x + dirs[i][0];
        int ny = pos.y + dirs[i][1];

        if (nx < 0 || ny < 0 || nx >= BOARD_SIZE || ny >= BOARD_SIZE)
            continue;

        if (board->groups[ny][nx] == NULL) { // empty intersection
            if (count >= capacity) {
                capacity *= 2;
                *out_liberties = realloc(*out_liberties, capacity * sizeof(Pos));
            }
            (*out_liberties)[count++] = (Pos){nx, ny};
        }
        else {
            remove_liberty(board->groups[ny][nx], pos);
        }
    }

    return count;
}

int remove_liberty(Group *group, Pos to_remove) {
    for (int i = 0; i < group->liberty_count; i++) {
        if (group->liberties[i].x == to_remove.x && group->liberties[i].y == to_remove.y) {
            // Shift the rest of the liberties left by one
            for (int j = i; j < group->liberty_count - 1; j++) {
                group->liberties[j] = group->liberties[j + 1];
            }
            group->liberty_count--;
            return 0; // successfully removed
        }
    }
    return 1; // not found
}



// Remove duplicates in-place and return new count
int removeDuplicateLiberties(Pos *liberties, int count) {
    if (count <= 1) {
        return count;
    }
    int new_count = 0;
    for (int i = 0; i < count; i++) {
        bool duplicate = false;
        for (int j = 0; j < new_count; j++) {
            if (liberties[i].x == liberties[j].x && liberties[i].y == liberties[j].y) {
                duplicate = true;
                break;
            }
        }
        if (!duplicate) {
            liberties[new_count++] = liberties[i];
        }
    }
    return new_count;
}




// ! ------------------------------------------------- Needs to probably deep copy groups as well ----- Might not be useful

int **copy_board(Board *board) {
    Board *copy = create_board();
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            copy->board[i][j] = board->board[i][j];
        }
    }

}

// ! ------------

// places a move on the board if the move is valid
// players are represented by a number:
// black: 1
// white: 2
int place_stone(Board *board, Player player, Pos pos) {
    if (is_move_valid(board, player, pos)) {
        board->board[pos.y][pos.x] = player.num;
        return 0;
    }
    return 1;
}

bool is_move_valid(Board *board, Player player, Pos pos) {
    if (pos.x < 0 || pos.x > BOARD_SIZE || pos.y < 0 || pos.y > BOARD_SIZE) {
        return false;
    }
    if (board->board[pos.y][pos.x] != 0) {
        return false;
    }
    Pos *new_libs;
    if (getLiberties(board, pos, &new_libs) == 0) {
        free(new_libs);
        return false;
    }
    return true;
}

int captures(Board *board, Pos move, Player player) {
    int dirs[4][2] = {{0,1},{1,0},{0,-1},{-1,0}}; // up, right, down, left
    int count = 0;

    for (int i = 0; i < 4; i++) {
        int nx = move.x + dirs[i][0];
        int ny = move.y + dirs[i][1];

        if (nx < 0 || ny < 0 || nx >= BOARD_SIZE || ny >= BOARD_SIZE) {
            continue;
        }

        Group *neighbor = board->groups[ny][nx];
        if (neighbor != NULL) {
            if (neighbor->liberty_count == 0) {
                count += neighbor->size;
                remove_from_board(board, neighbor);
            }
        }
    }
    return count;
}

// returns the number of captured stones
int remove_from_board(Board *board, Group *captured) {
    int dirs[4][2] = {{0,1},{1,0},{0,-1},{-1,0}};

    for (int i = 0; i < captured->size; i++) {
        Pos pos = captured->stones[i];
        board->board[pos.y][pos.x] = 0;
        board->groups[pos.y][pos.x] = NULL;

        for (int d = 0; d < 4; d++) {
            int nx = pos.x + dirs[d][0];
            int ny = pos.y + dirs[d][1];

            if (nx < 0 || ny < 0 || nx >= BOARD_SIZE || ny >= BOARD_SIZE)
                continue;

            Group *neighbor = board->groups[ny][nx];
            if (neighbor == NULL) {
                continue; 
            }

            if (!libertyExists(neighbor->liberties, neighbor->liberty_count, pos)) {
                if (neighbor->liberty_count >= neighbor->liberty_capacity) {
                    neighbor->liberty_capacity *= 2;
                    neighbor->liberties = realloc(neighbor->liberties, neighbor->liberty_capacity * sizeof(Pos));
                }
                neighbor->liberties[neighbor->liberty_count++] = pos;
            }
        }
    }
    int size = captured->size;
    
    free(captured->stones);
    free(captured->liberties);
    free(captured);

    return size;
}


bool compare_boards(Board *board_1, Board *board_2) {
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
        if (board_1->board[i / BOARD_SIZE][i % BOARD_SIZE] != board_2->board[i / BOARD_SIZE][i % BOARD_SIZE]) {
            return false;
        }
    }
    return true;
}
