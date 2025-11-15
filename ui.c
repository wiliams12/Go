#include "ui.h"
#include <math.h>
#include <stdio.h>

int tile_size = WINDOW_SIZE / (BOARD_SIZE + 1);

int draw_board(Board *board) {
    ClearBackground((Color){214, 181, 105});

    for (int i = 0; i < BOARD_SIZE; i++) {
        DrawLine(tile_size, tile_size*(i+1), WINDOW_SIZE-tile_size, tile_size*(i+1), BLACK);
        DrawLine(tile_size*(i+1), tile_size, tile_size*(i+1), WINDOW_SIZE-tile_size, BLACK);
    }

    draw_hoshi(BOARD_SIZE, tile_size);

    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
        int current = board->board[i / BOARD_SIZE][i % BOARD_SIZE];
        draw_stone(board, (Pos){i % BOARD_SIZE, i / BOARD_SIZE});
    }

    return 0;

}

void draw_hoshi(int board_size, float tile_size) {
    int hoshi[9][2]; // max 9 points
    int count = 0;

    if (board_size == 19) {
        int pos[] = {3, 9, 15}; // 0-indexed
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                hoshi[count][0] = pos[i], hoshi[count++][1] = pos[j];
    } else if (board_size == 13) {
        // corners + center (5 points)
        int corner_center[5][2] = {{3,3},{3,9},{6,6},{9,3},{9,9}};
        for (int i = 0; i < 5; i++)
            hoshi[i][0] = corner_center[i][0], hoshi[i][1] = corner_center[i][1];
        count = 5;
    } else if (board_size == 9) {
        // corners + center (5 points)
        int corner_center[5][2] = {{2,2},{2,6},{4,4},{6,2},{6,6}};
        for (int i = 0; i < 5; i++)
            hoshi[i][0] = corner_center[i][0], hoshi[i][1] = corner_center[i][1];
        count = 5;
    } else {
        // Default: corners + center (5 points)
        hoshi[0][0] = 0; hoshi[0][1] = 0;
        hoshi[1][0] = 0; hoshi[1][1] = board_size-1;
        hoshi[2][0] = board_size-1; hoshi[2][1] = 0;
        hoshi[3][0] = board_size-1; hoshi[3][1] = board_size-1;
        hoshi[4][0] = board_size/2; hoshi[4][1] = board_size/2;
        count = 5;
    }

    for (int i = 0; i < count; i++) {
        DrawCircle(tile_size * (hoshi[i][0] + 1),
                   tile_size * (hoshi[i][1] + 1),
                   tile_size / 16,
                   BLACK);
    }
}



int draw_stone(Board *board, Pos pos) {
    Color color;
    if (board->board[pos.y][pos.x] == 1) {
        color = BLACK;
    } else if (board->board[pos.y][pos.x] == 2){
        color = WHITE;
    } else {
        return 1;
    }

    DrawCircle(tile_size*(pos.x+1), tile_size*(pos.y+1), tile_size/2 - 1, color);
    
    return 0;
}

Pos get_move(void) {
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

void draw_bar(Player *players, Bar *bar, int turn) {
    DrawRectangleRec(bar->pass_btn, players[turn % 2].color);
    DrawTextCentered("Pass", bar->pass_btn, BAR_SIZE / 2, players[(turn + 1) % 2].color);
    DrawRectangleRec(bar->reset_btn, players[turn % 2].color);
    DrawTextCentered("Reset", bar->reset_btn, BAR_SIZE / 2, players[(turn + 1) % 2].color);
    
    DrawCircle(bar->white_score_rect.x + bar->white_score_rect.width / 2,
    bar->white_score_rect.y + bar->white_score_rect.height / 2,
    bar->white_score_rect.width / 3, WHITE);

    char buffer[16];
    sprintf(buffer, "%.1f", players[1].score);
    DrawTextCentered(buffer, bar->white_score_rect, BAR_SIZE / 2, BLACK);

    DrawCircle(bar->black_score_rect.x + bar->black_score_rect.width / 2,
    bar->black_score_rect.y + bar->black_score_rect.height / 2,
    bar->black_score_rect.width / 3, BLACK);

    sprintf(buffer, "%.1f", players[0].score);
    DrawTextCentered(buffer, bar->black_score_rect, BAR_SIZE / 2, WHITE);
    
}