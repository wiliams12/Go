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

// ! Undo moves is more effiecient though
Board *copy_board(Board *src) {
    if (!src) return NULL;

    Board *dst = malloc(sizeof(Board));
    if (!dst) return NULL;

    dst->move_num = src->move_num;
    dst->hash = src->hash;
    dst->history_capacity = src->history_capacity;

    // --- Copy board array ---
    dst->board = malloc(sizeof(int*) * BOARD_SIZE);
    for (int y = 0; y < BOARD_SIZE; y++) {
        dst->board[y] = malloc(sizeof(int) * BOARD_SIZE);
        for (int x = 0; x < BOARD_SIZE; x++) {
            dst->board[y][x] = src->board[y][x];
        }
    }

    // --- Allocate groups grid ---
    dst->groups = malloc(sizeof(Group**) * BOARD_SIZE);
    for (int y = 0; y < BOARD_SIZE; y++) {
        dst->groups[y] = malloc(sizeof(Group*) * BOARD_SIZE);
        for (int x = 0; x < BOARD_SIZE; x++)
            dst->groups[y][x] = NULL;
    }

    printf("1\n");

    // --- Mapping old group pointer → new group pointer ---
    Group *old_groups[BOARD_SIZE * BOARD_SIZE];
    Group *new_groups[BOARD_SIZE * BOARD_SIZE];
    int map_count = 0;

    // --- Copy all groups ---
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            Group *g = src->groups[y][x];
            if (!g) continue;

            Group *copy = NULL;

            // check mapping
            for (int m = 0; m < map_count; m++) {
                if (old_groups[m] == g) {
                    copy = new_groups[m];
                    break;
                }
            }

            if (!copy) {
                // Create new group
                copy = malloc(sizeof(Group));
                copy->capacity = g->capacity;
                copy->size = g->size;
                copy->stones = malloc(sizeof(Pos) * g->capacity);
                for (int i = 0; i < g->size; i++) {
                    copy->stones[i] = g->stones[i];
                }

                copy->liberty_capacity = g->liberty_capacity;
                copy->liberty_count = g->liberty_count;
                copy->liberties = malloc(sizeof(Pos) * g->liberty_capacity);
                for (int i = 0; i < g->liberty_count; i++) {
                    copy->liberties[i] = g->liberties[i];
                }
                old_groups[map_count] = g;
                new_groups[map_count] = copy;
                map_count++;
            }

            dst->groups[y][x] = copy;
        }
    }

    printf("2\n");

    // --- Copy hash table ---
    dst->hash_table = malloc(sizeof(uint64_t) * (BOARD_SIZE * BOARD_SIZE * 2));
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE * 2; i++) {
        dst->hash_table[i] = src->hash_table[i];
    }

    // --- Copy board history ---
    dst->board_history = malloc(sizeof(uint64_t) * dst->history_capacity);
    for (int i = 0; i < src->move_num; i++) {
        dst->board_history[i] = src->board_history[i];
    }

    // --- Copy moves list ---
    dst->moves = malloc(sizeof(Pos) * dst->history_capacity);
    for (int i = 0; i < src->move_num; i++) {
        dst->moves[i] = src->moves[i];
    }

    printf("3\n");

    // --- Copy captured groups ---
    dst->captured = malloc(sizeof(Group*) * dst->history_capacity);

    for (int i = 0; i < dst->history_capacity; i++) {
        Group *c = src->captured[i];

        if (!c) {
            dst->captured[i] = NULL;
            continue;
        }

        printf("not capturd overflow\n");

        // Deep copy captured group
        Group *newc = malloc(sizeof(Group));
        newc->capacity = c->capacity;
        newc->size = c->size;
        newc->stones = malloc(sizeof(Pos) * c->capacity);
        for (int j = 0; j < c->size; j++) {
            newc->stones[j] = c->stones[j];
        }

        newc->liberty_capacity = c->liberty_capacity;
        newc->liberty_count = c->liberty_count;
        newc->liberties = malloc(sizeof(Pos) * c->liberty_capacity);
        for (int j = 0; j < c->liberty_count; j++) {
            newc->liberties[j] = c->liberties[j];
        }

        dst->captured[i] = newc;
    }

    printf("4\n");

    return dst;
}







/*
Liberty handling & merged groups
Adding the undone point back to neighbor groups must happen after any captured groups are restored (undo_capture), 
and you must consider that neighbor groups may have been merged or re-created by undo_capture.
*/

// ! when a group is destroyed by getting rid of the dividing stone, it doesn't separate it into different groups

bool undo_move(Board *board) {
    // ! issues with switching players
    if (board->move_num <= 0) {
        return false;
    }
        
    if (board->move_num < 2) {
        board->hash = 0;
    }

    Pos last = board->moves[board->move_num - 1];
    int last_color = board->board[last.y][last.x];
    update_hash(board, last, last_color);
    board->board[last.y][last.x] = 0;
    board->groups[last.y][last.x] = NULL;

    Group *cap = board->captured[board->move_num - 1];
    if (cap != NULL) {
        // restore all stones in the captured group
        undo_capture(board, cap);
    }

    if (cap != NULL) {
        free(cap->stones);
        free(cap->liberties);
        free(cap);
        board->captured[board->move_num - 1] = NULL;
    }

    Pos dirs[4] = {
        { last.x + 1, last.y },
        { last.x - 1, last.y },
        { last.x, last.y + 1 },
        { last.x, last.y - 1 }
    };
    
    // ! wrong down from here

    // Track rebuilt stones
    bool visited[BOARD_SIZE][BOARD_SIZE] = {0};

    Pos neighs[4] = {
        { last.x+1, last.y },
        { last.x-1, last.y },
        { last.x,   last.y+1 },
        { last.x,   last.y-1 }
    };

    for (int i = 0; i < 4; i++) {
        int nx = neighs[i].x;
        int ny = neighs[i].y;

        if (nx < 0 || nx >= BOARD_SIZE || ny < 0 || ny >= BOARD_SIZE)
            continue;
        if (board->board[ny][nx] != last_color)
            continue;
        if (visited[ny][nx])
            continue;

        // === BFS to collect this connected component ===
        Pos stack[BOARD_SIZE * BOARD_SIZE];
        int sp = 0;

        Pos component[BOARD_SIZE * BOARD_SIZE];
        int comp_len = 0;

        visited[ny][nx] = true;
        stack[sp++] = (Pos){nx, ny};

        while (sp > 0) {
            Pos p = stack[--sp];
            component[comp_len++] = p;

            Pos dirs2[4] = {
                { p.x+1, p.y },
                { p.x-1, p.y },
                { p.x,   p.y+1 },
                { p.x,   p.y-1 }
            };

            for (int d = 0; d < 4; d++) {
                int xx = dirs2[d].x;
                int yy = dirs2[d].y;

                if (xx < 0 || xx >= BOARD_SIZE || yy < 0 || yy >= BOARD_SIZE)
                    continue;

                if (board->board[yy][xx] != last_color)
                    continue;

                if (!visited[yy][xx]) {
                    visited[yy][xx] = true;
                    stack[sp++] = (Pos){xx, yy};
                }
            }
        }

        // === NOW, rebuild this component stone-by-stone ===
        for (int s = 0; s < comp_len; s++) {
            Pos p = component[s];

            // assign NULL first
            board->groups[p.y][p.x] = NULL;

            // try to merge
            if (!mergeWithAdjacentGroups(board, p, last_color)) {
                // create new group
                Group *g = malloc(sizeof(Group));
                initGroup(g);
                board->groups[p.y][p.x] = g;
                addStoneToGroup(board, g, p);
            }
        }
    }

    board->move_num--;

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

