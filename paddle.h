
typedef struct Paddle {
    Rectangle rect;
    float speed;
    int key_up, key_down; // Key binds for moving
    int score;
} Paddle;

void paddle_init(Paddle* paddle, bool is_left, float speed, int key_up, int key_down)
{
    const int EDGE_OFFSET = 32;
    int width = 16;
    int height = 75;
    int x = is_left ? EDGE_OFFSET : GetScreenWidth() - width - EDGE_OFFSET;

    paddle->rect = (Rectangle) { x, GetScreenHeight()/2 - height/2, width, height };
    paddle->speed = speed;
    paddle->key_up = key_up;
    paddle->key_down = key_down;
    paddle->score = 0;
}

void draw_paddle(const Paddle* paddle)
{
    DrawRectangle(paddle->rect.x, paddle->rect.y, paddle->rect.width, paddle->rect.height, WHITE);
}

void update_paddle(Paddle* paddle)
{
    const int EDGE_HEIGHT = 16;

    if (IsKeyDown(paddle->key_up)) {
        paddle->rect.y -= paddle->speed * GetFrameTime();
        if (paddle->rect.y < EDGE_HEIGHT) 
            paddle->rect.y = EDGE_HEIGHT;
    }

    if (IsKeyDown(paddle->key_down)) {
        paddle->rect.y += paddle->speed * GetFrameTime();
        if (paddle->rect.y > GetScreenHeight() - paddle->rect.height - EDGE_HEIGHT) 
            paddle->rect.y = GetScreenHeight() - paddle->rect.height - EDGE_HEIGHT;
    }
}