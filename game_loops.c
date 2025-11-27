#include"game_loops.h"
#include"hash.h"


int two_players(Board *board) {
    bool running = true;
    Player players[2] = {(Player){.color=BLACK, .captured=0, .num=1, .score=0.0f}, (Player){.color=WHITE, .captured=0, .num=2, .score=0.0f}};
    int turn = 0;

    Bar *bar = malloc(sizeof(Bar));


    bar->pass_btn = (Rectangle){ WINDOW_SIZE / 8, WINDOW_SIZE - WINDOW_SIZE / BOARD_SIZE + BAR_SIZE / 2, WINDOW_SIZE / 8, BAR_SIZE };
    bar->reset_btn = (Rectangle){ WINDOW_SIZE * 6/ 8, WINDOW_SIZE - WINDOW_SIZE / BOARD_SIZE + BAR_SIZE / 2, WINDOW_SIZE / 8, BAR_SIZE};
    bar->white_score_rect = (Rectangle){ WINDOW_SIZE * 7/ 8 + MARGIN, WINDOW_SIZE - WINDOW_SIZE / BOARD_SIZE + BAR_SIZE / 2, WINDOW_SIZE / 8 - MARGIN * 2, BAR_SIZE};
    bar->black_score_rect = (Rectangle){MARGIN, WINDOW_SIZE - WINDOW_SIZE / BOARD_SIZE + BAR_SIZE / 2, WINDOW_SIZE / 8 - MARGIN * 2, BAR_SIZE};

    while (!WindowShouldClose()) {
        running = true;
        while (running && !WindowShouldClose()) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mouse = GetMousePosition();

                if (CheckCollisionPointRec(mouse, bar->pass_btn)) {
                    update_board_history(board, (Pos){.x=-1, .y=1});  
                    running = false;
                } else if (CheckCollisionPointRec(mouse, bar->reset_btn)) {
                    turn = 0;
                    board = create_board();
                    players[0].score = 0;
                    players[1].score = 0;
                } else {
                    Pos move = get_move();
                    if (play_move(board, move, players)) {
                        running = false;
                    }
                }
            }
            BeginDrawing();

            draw_board(board);
            draw_bar(players, bar, turn);
            EndDrawing();
        }
        turn++;
        if (is_over(board, players)) {
            break;
        }
    }
    CloseWindow();
    return 0;
}


int computer(Board *board) {
    bool running = true;
    Player players[2] = {(Player){.color=BLACK, .captured=0, .num=1, .score=0.0f}, (Player){.color=WHITE, .captured=0, .num=2, .score=0.0f}};
    int turn = 0;

    Bar *bar = malloc(sizeof(Bar));

    bar->pass_btn = (Rectangle){ WINDOW_SIZE / 8, WINDOW_SIZE - WINDOW_SIZE / BOARD_SIZE + BAR_SIZE / 2, WINDOW_SIZE / 8, BAR_SIZE };
    bar->reset_btn = (Rectangle){ WINDOW_SIZE * 6/ 8, WINDOW_SIZE - WINDOW_SIZE / BOARD_SIZE + BAR_SIZE / 2, WINDOW_SIZE / 8, BAR_SIZE};
    bar->white_score_rect = (Rectangle){ WINDOW_SIZE * 7/ 8 + MARGIN, WINDOW_SIZE - WINDOW_SIZE / BOARD_SIZE + BAR_SIZE / 2, WINDOW_SIZE / 8 - MARGIN * 2, BAR_SIZE};
    bar->black_score_rect = (Rectangle){MARGIN, WINDOW_SIZE - WINDOW_SIZE / BOARD_SIZE + BAR_SIZE / 2, WINDOW_SIZE / 8 - MARGIN * 2, BAR_SIZE};

    while (!WindowShouldClose()) {
        running = true;
        if (turn % 2) {
            while (running && !WindowShouldClose()) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    Vector2 mouse = GetMousePosition();
    
                    if (CheckCollisionPointRec(mouse, bar->pass_btn)) {
                        update_board_history(board, (Pos){.x=-1, .y=1});  
                        running = false;
                    } else if (CheckCollisionPointRec(mouse, bar->reset_btn)) {
                        turn = 0;
                        board = create_board();
                        players[0].score = 0;
                        players[1].score = 0;
                    } else {
                        Pos move = get_move();
                        if (play_move(board, move, players)) {
                            running = false;
                        }
                    }
                }
                BeginDrawing();
    
                draw_board(board);
                draw_bar(players, bar, turn);
                EndDrawing();
            }
            turn++;
        } else {
            Node *root = malloc(sizeof(Node));
            root->pos = (Pos){ .x = -1, .y = -1 };   // root has no move
            root->visits = 0;
            root->wins = 0;
            root->children = NULL;
            root->children_count = 0;
            root->parent = NULL; 
            Pos move = mcts_get_best_move(board, root, players, 10000);
            // ! Fucks up liberties
            // ! Fucks up color turn
            if (move.x != -1 && move.y != -1) {
                play_move(board, move, players);
            }
            turn++;
        }
        if (is_over(board, players)) {
            break;
        }
    }
    CloseWindow();
    return 0;
}


