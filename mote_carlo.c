
#include"monte_carlo.h"

#define TOTAL_VISITS 1000.0f 
#define FLT_MAX 100.0f
#define EXPLORATION_PAR 1.4f

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

float selection(Node *moves, int numMoves, Board *board) {
    float bestValue = -BOARD_SIZE * BOARD_SIZE;
    int bestIndex = -1;

    for (int i = 0; i < numMoves; i++) {
        float policy = policyNetwork(moves[i], board);

        float score = policy + (move.wins/move.visits+1) +
                    EXPLORATION_PAR * sqrt(log(TOTAL_VISITS) / (1 + moves[i].visits));

        if (score > bestValue) {
            bestValue = score;
            bestIndex = i;
        }
    }

    return bestValue;
}


int get_children(Board *board, Node **nodes) {
    Pos *moves = malloc(sizeof(Pos) * BOARD_SIZE * BOARD_SIZE);
    int found = get_legal_moves(board, moves, board->players[board->move_num % 2]);
    for (int i = 0; i < found; i++) {
        nodes[i]->pos = moves[found];
        nodes[i]->visits = 0;
        nodes[i]->wins = 0;
        nodes[i]->children = NULL;
        nodes[i]->children_count = 0;
    }
    free(moves);
}


// ! free everything after use
int expansion(Node *parent, Board *board) {
    if (parent->children != NULL) {
        return parent->children_count;
    }

    parent->children = malloc(sizeof(Node*) * BOARD_SIZE * BOARD_SIZE);
    parent->children_count = 0;

    Node *moves = malloc(sizeof(Node) * BOARD_SIZE * BOARD_SIZE);
    int num_moves = get_children(board, moves);

    for (int i = 0; i < num_moves; i++) {
        parent->children[i] = moves[i];
        parent->children_count++;
    }

    return parent->children_count;
}


int simulation(Board *board, Player *players) {
    int simulated = 0;
    int root_player = board->move_num;

    while (!is_over(board, players)) {

        Pos *moves = malloc(sizeof(Pos) * BOARD_SIZE * BOARD_SIZE);
        int count = get_legal_moves(baord, moves, players[board->move_num % 2]);

        int idx = rand() % count;
        play_move(board, moves[idx], players);
        simulated++;
        free(moves);
    }

    int black_score = 0;
    int white_score = 0;
    score_board(board, &black_score, &white_score);

    int delta = black_score - white_score;
    // draw shouldn't happen, but better to have everything covered
    int result = 0;

    if (players[root_player % 2]) {
        if (delta > 0) {
            result = 1;
        } else if (delta < 0) {
            result = -1;
        } else {
            printf("internal error: simulation ended in a draw")
        }
    } else {
        if (delta > 0) {
            result = -1;
        } else if (delta < 0) {
            result = 1;
        } else {
            printf("internal error: simulation ended in a draw")
        }
    }

    for (int i = simulated - 1; i >= 0; i--) {
        undo_move(board);
    }

    return result;
}


int backpropagation() {
    
}

