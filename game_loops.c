#include"game_loops.h"
#include"hash.h"

int two_players(Board *board) {
    bool running = true;
    Player players[2] = {(Player){BLACK, 0, 1, 0}, (Player){WHITE, 0, 2, 0}};
    int turn = 0;

    Rectangle pass_btn = { WINDOW_SIZE / 8, WINDOW_SIZE - WINDOW_SIZE / BOARD_SIZE + BAR_SIZE / 2, WINDOW_SIZE / 8, BAR_SIZE };
    Rectangle reset_btn = { WINDOW_SIZE * 6/ 8, WINDOW_SIZE - WINDOW_SIZE / BOARD_SIZE + BAR_SIZE / 2, WINDOW_SIZE / 8, BAR_SIZE};
    Rectangle white_score_rect = { WINDOW_SIZE * 7/ 8 + MARGIN, WINDOW_SIZE - WINDOW_SIZE / BOARD_SIZE + BAR_SIZE / 2, WINDOW_SIZE / 8 - MARGIN * 2, BAR_SIZE};
    Rectangle black_score_rect = {MARGIN, WINDOW_SIZE - WINDOW_SIZE / BOARD_SIZE + BAR_SIZE / 2, WINDOW_SIZE / 8 - MARGIN * 2, BAR_SIZE};

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

                Pos move = get_move();
                if (place_stone(board, players[turn % 2], move) == 0) {
                    running = false;

                    if (!mergeWithAdjacentGroups(board, move, players[turn % 2].num)) {
                        Group *group = malloc(sizeof(Group));
                        initGroup(group);
                        board->groups[move.y][move.x] = group;
                        addStoneToGroup(board, group, move);
                    }

                    update_hash(board, move, players[turn % 2].num);
                    update_board_history(board);

                    players[(turn + 1) % 2].score += captures(board, move, players[turn % 2]);
                };
            }
            BeginDrawing();

            draw_board(board);
            DrawRectangleRec(pass_btn, players[turn % 2].color);
            DrawTextCentered("Pass", pass_btn, BAR_SIZE / 2, players[(turn + 1) % 2].color);
            DrawRectangleRec(reset_btn, players[turn % 2].color);
            DrawTextCentered("Reset", reset_btn, BAR_SIZE / 2, players[(turn + 1) % 2].color);
            
            DrawCircle(white_score_rect.x + white_score_rect.width / 2,
           white_score_rect.y + white_score_rect.height / 2,
           white_score_rect.width / 3, WHITE);

            char buffer[16];
            sprintf(buffer, "%d", players[1].score);
            DrawTextCentered(buffer, white_score_rect, BAR_SIZE / 2, BLACK);

            DrawCircle(black_score_rect.x + black_score_rect.width / 2,
           black_score_rect.y + black_score_rect.height / 2,
           black_score_rect.width / 3, BLACK);

            sprintf(buffer, "%d", players[0].score);
            DrawTextCentered(buffer, black_score_rect, BAR_SIZE / 2, WHITE);
            
            EndDrawing();
        }
        turn++;
    }
    CloseWindow();
    return 0;
}