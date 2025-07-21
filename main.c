#include <raylib.h>
#include "paddle.h"
#include "ball.h"

int main()
{
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 960;

    InitAudioDevice();
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong");

    bool debug_mode = false;

    // Init ball
    Ball ball;
    ball_init(&ball, 16, 500.f, 1200.f, 50.f);
    
    // Init paddles
    Paddle paddle_left, paddle_right;
    paddle_init(&paddle_left, true, 600.f, KEY_W, KEY_S);
    paddle_init(&paddle_right, false, 600.f, KEY_UP, KEY_DOWN);

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        update_paddle(&paddle_left);
        update_paddle(&paddle_right);

        update_ball(&ball);
        ball_collision(&ball, &paddle_left, true);
        ball_collision(&ball, &paddle_right, false);

        update_score(&ball, &paddle_left, &paddle_right);

        if (IsKeyPressed(KEY_F1)) debug_mode = !debug_mode;

        // Draw
        BeginDrawing();

            ClearBackground(BLACK);
            
            // Draw edges
            DrawRectangle(0, 0, SCREEN_WIDTH, 16, WHITE);
            DrawRectangle(0, SCREEN_HEIGHT-16, SCREEN_WIDTH, 16, WHITE);

            for (int i = 0; i < 29; i++) {
                DrawRectangle(SCREEN_WIDTH/2-8, 24 + i*32, 16, 16, WHITE);
            }

            draw_paddle(&paddle_left);
            draw_paddle(&paddle_right);
            draw_ball(&ball);

            // Draw score
            const char* score_left = TextFormat("%i", paddle_left.score);
            const char* score_right = TextFormat("%i", paddle_right.score);
            const int FONT_SIZE = 64;

            DrawText(score_left, SCREEN_WIDTH*0.25-MeasureText(score_left, FONT_SIZE)/2, 32, FONT_SIZE, WHITE);
            DrawText(score_right, SCREEN_WIDTH*0.75-MeasureText(score_right, FONT_SIZE)/2, 32, FONT_SIZE, WHITE);

            // Draw debug info
            if (debug_mode) {
                DrawText(TextFormat("FPS: %i\n\nSpeed: %.0f", GetFPS(), ball.speed), 16, 32, 32, GREEN);
            }

        EndDrawing();
    }
    CloseWindow();
}