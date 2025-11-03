#include"game_loops.h"

int two_players(int **board) {
    bool running = true;
    int player = 1;
    while (!WindowShouldClose()) {
        running = true;
        while (running && !WindowShouldClose()) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Pos move = get_move(board);
                if (place_stone(board, player, move) == 0) {
                    running = false;
                };
            }
            BeginDrawing();
            draw_board(board, player);
            EndDrawing();
        }
        player = (player == 1) ? 2 : 1;
    }
    // TODO:
        // update to be better

        CloseWindow();
        return 0;
}