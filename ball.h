#include <raylib.h>
#include <math.h>

typedef struct Ball {
    Rectangle rect;
    int size;
    float speed;
    float min_speed, max_speed;
    float speed_x, speed_y;
    float speed_increase;
    Sound sound_bounce;
    Sound sound_scored;
} Ball;

void ball_init(Ball* ball, int size, float min_speed, float max_speed, float speed_increase)
{
    ball->size = size;
    ball->min_speed = min_speed;
    ball->max_speed = max_speed;
    ball->speed_increase = speed_increase;
    ball->speed = min_speed;
    
    ball->speed_x = min_speed;
    ball->speed_y = 0;
    ball->rect = (Rectangle) { GetScreenWidth()/2 - size/2, GetScreenHeight()/2 - size/2, size, size };

    ball->sound_bounce = LoadSound("sounds/bounce.wav");
    ball->sound_scored = LoadSound("sounds/scored.wav");
}

void draw_ball(const Ball* ball)
{
    DrawRectangle(ball->rect.x, ball->rect.y, ball->rect.width, ball->rect.height, WHITE);
}

void update_ball(Ball* ball)
{
    // Move
    ball->rect.x += ball->speed_x * GetFrameTime();
    ball->rect.y += ball->speed_y * GetFrameTime();

    // Collision with the edges
    if ((ball->rect.y <= 16 && ball->speed_y < 0) ||
        (ball->rect.y >= GetScreenHeight() - 32 && ball->speed_y > 0)) 
    {
        ball->speed_y *= -1;
        PlaySound(ball->sound_bounce);
    }
}

void ball_collision(Ball* ball, Paddle* paddle, bool is_left)
{   
    const float MAX_BOUNCE_ANGLE = PI / 3.f;

    if (CheckCollisionRecs(ball->rect, paddle->rect)) {
        // Increase the speed of the ball
        if (((is_left && ball->speed_x < 0) || (!is_left && ball->speed_x > 0)) &&
              ball->speed < ball->max_speed)
        {
            ball->speed += ball->speed_increase;
            PlaySound(ball->sound_bounce);
        }

        // The bounce angle is larger with the distance from the center of the paddle
        int center_paddle = paddle->rect.y + paddle->rect.height/2;
        int center_ball = ball->rect.y + ball->rect.height/2;
        
        float max_distance = paddle->rect.height/2 + ball->rect.height/2;
        float distance = (center_ball - center_paddle) / max_distance; // values <-1; 1>
        float angle = 2*PI - distance * MAX_BOUNCE_ANGLE;

        ball->speed_x = ball->speed * -cos(angle);
        if (is_left) ball->speed_x *= -1;
        ball->speed_y = ball->speed * -sin(angle);
    }
}

void reset_ball(Ball* ball)
{
    ball->rect.x = GetScreenWidth()/2 - ball->rect.width/2;
    
    ball->speed = ball->min_speed;
    ball->speed_x = (ball->speed_x > 0) ? ball->min_speed : -ball->min_speed;
    ball->speed_y = 0;

    PlaySound(ball->sound_scored);
}

void update_score(Ball* ball, Paddle* paddle_left, Paddle* paddle_right)
{
    if (ball->rect.x + ball->rect.width < 0) {
        paddle_right->score++;
        reset_ball(ball);
    }
    else if (ball->rect.x - ball->rect.width > GetScreenWidth()) {
        paddle_left->score++;
        reset_ball(ball);
    }
}