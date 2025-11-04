#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<raylib.h>

#ifndef COMMON
#define COMMON
#define FPS 30

#define WINDOW_SIZE 700
#define BAR_SIZE 50

typedef struct {
    int x;
    int y;
} Pos;

typedef struct {
    Color color;
    int captured;
    int num;
} Player;

#endif