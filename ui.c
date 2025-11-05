#include "ui.h"
#include <math.h>
#include <stdio.h>

int tile_size = WINDOW_SIZE / (BOARD_SIZE + 1);

int draw_board(int **board) {
    ClearBackground((Color){214, 181, 105});

    for (int i = 0; i < BOARD_SIZE; i++) {
        DrawLine(tile_size, tile_size*(i+1), WINDOW_SIZE-tile_size, tile_size*(i+1), BLACK);
        DrawLine(tile_size*(i+1), tile_size, tile_size*(i+1), WINDOW_SIZE-tile_size, BLACK);
    }

    for (int i = 0; i < 9; i++) {
        DrawCircle(tile_size*(4+(i/3) * 6), tile_size*(4+(i%3) * 6), tile_size/16, BLACK);
    }

    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
        int current = board[i / BOARD_SIZE][i % BOARD_SIZE];
        draw_stone(current, (Pos){i % BOARD_SIZE, i / BOARD_SIZE});
    }

    return 0;
}

int draw_stone(int current_player, Pos pos) {
    Color color;
    if (current_player == 1) {
        color = BLACK;
    } else if (current_player == 2){
        color = WHITE;
    } else {
        return 1;
    }

    DrawCircle(tile_size*(pos.x+1), tile_size*(pos.y+1), tile_size/2 - 1, color);
    
    return 0;
}

Pos get_move(int **board) {
    Vector2 mouse_pos = GetMousePosition();
    Pos board_pos;
    board_pos.x = (int)roundf(mouse_pos.x / tile_size) - 1;
    board_pos.y = (int)roundf(mouse_pos.y / tile_size) - 1;
    return board_pos;
}

int set_up(void) {
    InitWindow(WINDOW_SIZE, WINDOW_SIZE + BAR_SIZE, "Go");
    SetTargetFPS(FPS);
    return 0;
}

void DrawTextCentered(const char *text, Rectangle rect, int fontSize, Color color) {
    int textWidth = MeasureText(text, fontSize);
    int textHeight = fontSize;

    int x = rect.x + (rect.width - textWidth) / 2;
    int y = rect.y + (rect.height - textHeight) / 2;

    DrawText(text, x, y, fontSize, color);
}