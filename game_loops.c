#include"game_loops.h"

int two_players(int **board, int **captured) {
    bool running = true;
    Player players[2] = {(Player){BLACK, 0, 1}, (Player){WHITE, 0, 2}};
    int turn = 0;

    Rectangle pass_btn = { WINDOW_SIZE / 8, WINDOW_SIZE - WINDOW_SIZE / BOARD_SIZE + BAR_SIZE / 2, WINDOW_SIZE / 8, BAR_SIZE };
    Rectangle reset_btn = { WINDOW_SIZE * 6/ 8, WINDOW_SIZE - WINDOW_SIZE / BOARD_SIZE + BAR_SIZE / 2, WINDOW_SIZE / 8, BAR_SIZE};
    Rectangle white_score_rect = { WINDOW_SIZE * 7/ 8 + MARGIN, WINDOW_SIZE - WINDOW_SIZE / BOARD_SIZE + BAR_SIZE / 2, WINDOW_SIZE / 8 - MARGIN * 2, BAR_SIZE};
    Rectangle black_score_rect = {MARGIN, WINDOW_SIZE - WINDOW_SIZE / BOARD_SIZE + BAR_SIZE / 2, WINDOW_SIZE / 8 - MARGIN * 2, BAR_SIZE};

    int white_score = 0;
    int black_score = 0;

    GroupList **groups;
    //initGroupList(groups);

    while (!WindowShouldClose()) {
        running = true;
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
                if (place_stone(board, players[turn % 2], move) == 0) {
                    running = false;
                    // TODO Do grouping here 
                    
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
            
            DrawRectangleRec(white_score_rect, WHITE);
            char buffer[16];
            sprintf(buffer, "%d", white_score);
            DrawTextCentered(buffer, white_score_rect, BAR_SIZE / 2, BLACK);

            DrawRectangleRec(black_score_rect, BLACK);
            sprintf(buffer, "%d", black_score);
            DrawTextCentered(buffer, black_score_rect, BAR_SIZE / 2, WHITE);
            
            EndDrawing();
        }
        turn++;
    }
    // TODO:
        // update to be better

        CloseWindow();
        return 0;
}