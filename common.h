#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<raylib.h>

#ifndef COMMON
#define COMMON
#define FPS 30

#define WINDOW_SIZE 700
#define BAR_SIZE 50
#define MARGIN 5

typedef struct {
    int x;
    int y;
} Pos;

typedef struct {
    Color color;
    int captured;
    int num;
} Player;

typedef struct {
    Pos *stones;
    int liberties;
    int capacity;
    int size;
} Group;

typedef struct {
    Group *groups;
    int capacity;
    int size;
} GroupList;

typedef struct {
    int **board;
    Group **groups;
} Board;


#endif