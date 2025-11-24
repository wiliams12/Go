
#include"monte_carlo.h"


/*
Explanation:
selection goes down the tree
When it reaches a terminal node, it expands
Then does a simulation from that node to find the result
Backpropagate and update values
*/


float policyNetwork(Node move, Board *board) {
    // For simplicity, random-ish score based on position
    return 1.0f / ((move.pos.x + 1) + (move.pos.y + 1));
}

int selection(Node **moves, int numMoves, Board *board) {
    float bestValue = -BOARD_SIZE * BOARD_SIZE;
    int bestIndex = -1;

    for (int i = 0; i < numMoves; i++) {
        float policy = policyNetwork(*moves[i], board);

        float score = policy + (moves[i]->wins/moves[i]->visits+1) +
                    EXPLORATION_PAR * sqrt(log(TOTAL_VISITS) / (1 + moves[i]->visits));

        if (score > bestValue) {
            bestValue = score;
            bestIndex = i;
        }
    }

    return bestIndex;
}



int get_children(Board *board, Node *nodes, Player *players) {
    Pos *moves = malloc(sizeof(Pos) * BOARD_SIZE * BOARD_SIZE);

    int found = get_legal_moves(board, moves, players);

    for (int i = 0; i < found; i++) {
        nodes[i].pos = moves[i];
        nodes[i].visits = 0;
        nodes[i].wins = 0;
        nodes[i].children = NULL;
        nodes[i].children_count = 0;
        nodes[i].parent = NULL; 
    }

    free(moves);
    return found;
}



// ! free everything after use
int expansion(Node *parent, Board *board, Player *players) {
    if (parent->children != NULL) {
        return parent->children_count;
    }

    parent->children = malloc(sizeof(Node*) * BOARD_SIZE * BOARD_SIZE);
    parent->children_count = 0;

    Node *moves = malloc(sizeof(Node) * BOARD_SIZE * BOARD_SIZE);
    int num_moves = get_children(board, moves, players);

    for (int i = 0; i < num_moves; i++) {

        Node *child = malloc(sizeof(Node));

        child->pos = moves[i].pos;
        child->visits = 0;
        child->wins = 0;
        child->children = NULL;
        child->children_count = 0;
        child->parent = parent;

        parent->children[i] = child;
        parent->children_count++;
    }

    free(moves);

    return parent->children_count;
}


int simulation(Board *board, Player *players) {
    int simulated = 0;
    int root_player = board->move_num;

    // ! Seg is here
    while (!is_over(board, players)) {

        Pos *moves = malloc(sizeof(Pos) * BOARD_SIZE * BOARD_SIZE);
        int count = get_legal_moves(board, moves, &players[board->move_num % 2]);

        int idx = rand() % count;
        if (!play_move(board, moves[idx], players)) {
            printf("internal error\n");
        }
        simulated++;
    }
    printf("left while\n");

    float black_score = 0;
    float white_score = 0;
    score_board(board, &black_score, &white_score);

    int delta = black_score - white_score;
    // draw shouldn't happen, but better to have everything covered
    int result = 0;

    if (root_player % 2) {
        if (delta > 0) {
            result = 1;
        } else if (delta < 0) {
            result = -1;
        } else {
            printf("internal error: simulation ended in a draw");
        }
    } else {
        if (delta > 0) {
            result = -1;
        } else if (delta < 0) {
            result = 1;
        } else {
            printf("internal error: simulation ended in a draw");
        }
    }

    for (int i = simulated - 1; i >= 0; i--) {
        undo_move(board);
    }

    return result;
}


void backpropagation(Node *leaf, int result) {
    Node *current = leaf;

    while (current != NULL) {
        current->visits++;

        if (result == 1) {
            current->wins++;
        }

        current = current->parent;
    }
}

// Returns the best move after running MCTS on root_state
Pos mcts_get_best_move(Board *board, Node *root_state, Player *players, int iterations) {
    // Run MCTS search
    mcts(board, root_state, players, iterations);

    printf("not in mcts\n");

    // No children = no legal moves (e.g., pass)
    if (root_state->children_count == 0) {
        return (Pos){ .x = -1, .y = -1 };
    }

    int best_idx = 0;
    int best_visits = root_state->children[0]->visits;

    // Pick child with highest visit count
    for (int i = 1; i < root_state->children_count; i++) {
        if (root_state->children[i]->visits > best_visits) {
            best_visits = root_state->children[i]->visits;
            best_idx = i;
        }
    }

    return root_state->children[best_idx]->pos;
}


void mcts(Board *board, Node *root_state, Player *players, int iterations) {

    int root_depth = board->move_num;

    for (int i = 0; i < iterations; i++) {
        Node *current_state = root_state;

        while (current_state->children != NULL &&
            current_state->children_count > 0) {
                
            printf("in selection\n");        
            int idx = selection(current_state->children,
                                current_state->children_count,
                                board);

            printf("not in selection\n");

            play_move(board, current_state->children[idx]->pos, players);

            current_state = current_state->children[idx];
        }

        if (!is_over(board, players)) {

            printf("in expansion\n");
            expansion(current_state, board, players);
            printf("not in expansion\n");

            // Choose one of the newly expanded children
            if (current_state->children_count > 0) {
                int idx = rand() % current_state->children_count;

                play_move(board, current_state->children[idx]->pos, players);

                current_state = current_state->children[idx];
            }
        }

        printf("in simualtion\n");
        int result = simulation(board, players);
        printf("not in simualtion\n");

        printf("in backpropagation\n");
        backpropagation(current_state, result);
        printf("not in backpropagation\n");

        while (board->move_num > root_depth) {
            undo_move(board);
        }
    }
}



