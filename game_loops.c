#include"game_loops.h"

int two_players(int **previous_board, int **board, int **captured) {
    bool running = true;
    Player players[2] = {(Player){BLACK, 0, 1}, (Player){WHITE, 0, 2}};
    int turn = 0;

    Rectangle pass_btn = { WINDOW_SIZE / 8, WINDOW_SIZE - WINDOW_SIZE / BOARD_SIZE + BAR_SIZE / 2, WINDOW_SIZE / 8, BAR_SIZE };
    Rectangle reset_btn = { WINDOW_SIZE * 6/ 8, WINDOW_SIZE - WINDOW_SIZE / BOARD_SIZE + BAR_SIZE / 2, WINDOW_SIZE / 8, BAR_SIZE};

    while (!WindowShouldClose()) {
        running = true;
        int *previous_move = NULL;
        while (running && !WindowShouldClose()) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mouse = GetMousePosition();

                if (CheckCollisionPointRec(mouse, pass_btn)) {
                    printf("Pass\n");
                }

                if (CheckCollisionPointRec(mouse, reset_btn)) {
                    printf("Reset\n");
                }

                Pos move = get_move(board);
                if (place_stone(previous_board, previous_move, board, players[turn % 2], move, players[(turn + 1) % 2]) == 0) {
                    running = false;
                    if (is_captured(board, captured, players[turn % 2])) {
                        int num_of_captured = remove_from_board(board, captured);
                    }
                };
            }
            BeginDrawing();

            draw_board(board);
            DrawRectangleRec(pass_btn, players[turn % 2].color);
            DrawTextCentered("Pass", pass_btn, BAR_SIZE / 2, players[(turn + 1) % 2].color);
            DrawRectangleRec(reset_btn, players[turn % 2].color);
            DrawTextCentered("Reset", reset_btn, BAR_SIZE / 2, players[(turn + 1) % 2].color);
            
            EndDrawing();
        }
        turn++;
    }
    // TODO:
        // update to be better

        CloseWindow();
        return 0;
}