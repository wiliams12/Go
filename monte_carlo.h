#include"common.h"
#include"board.h"

typedef struct {
    Pos pos;
    int visits;
    int wins;
    struct Node **children;
    int children_count;
} Node;

float policyNetwork(Node move, Board *board);

float selection(Node *moves, int numMoves, Board *board);

int get_children(Board *board, Node **nodes);

int expansion(Node *parent, Board *board);

int simulation(Node *start, Board *board, Player *players);

int backpropagation();

bool apply_move(Board *board, Node *move, Player *players);