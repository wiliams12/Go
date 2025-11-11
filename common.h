
#ifndef COMMON
#define COMMON
#define FPS 30

#define WINDOW_SIZE 700
#define BAR_SIZE 50
#define MARGIN 5

#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<raylib.h>
#include <time.h>

typedef struct {
    int x;
    int y;
} Pos;

typedef struct {
    Color color;
    int captured;
    int num;
    int score;
} Player;

typedef struct {
    Pos *stones;
    int capacity;
    int size;
    Pos *liberties;
    int liberty_count;
    int liberty_capacity;
} Group;


typedef struct {
    int **board;
    Group ***groups;
    uint64_t hash;
    uint64_t *hash_table;
    uint64_t *board_history;
    int history_size;
    int history_capacity;
} Board;


#endif