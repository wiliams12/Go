#ifndef MONTE_CARLO

#include"common.h"
#include"board.h"
#include"engine_helpers.h"

#include<math.h>

#define TOTAL_VISITS 1000.0f 
#define FLT_MAX 100.0f
#define EXPLORATION_PAR 1.4f

#endif
typedef struct Node {
    Pos pos;
    int visits;
    int wins;
    struct Node **children;
    int children_count;
    struct Node *parent; 
} Node;

float policyNetwork(Node move, Board *board);

int selection(Node **moves, int numMoves, Board *board);

int get_children(Board *board, Node *nodes, Player *players);

int expansion(Node *parent, Board *board, Player *players);

int simulation(Board *board, Player *players);

void backpropagation(Node *leaf, int result);

void mcts(Board *board, Node *root_state, Player *players, int iterations);

Pos mcts_get_best_move(Board *board, Node *root_state, Player *players, int iterations);