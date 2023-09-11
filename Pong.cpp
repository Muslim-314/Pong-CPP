#include <iostream>
#include <raylib.h>
#include"raymath.h"
#include"StartingImage.h"
#include"PongHeader.h"

#define screen_width 1100
#define screen_height 700 
Color Green = Color{ 38, 185, 154, 255 };
Color Dark_Green = Color{ 20, 160, 133, 255 };
Color Light_Green = Color{ 129, 204, 184, 255 };
Color Yellow = Color{ 243, 213, 91, 255 }; //ball colour

int player_score = 0;
int cpu_score = 0;
bool Paused = false;  //pause buttoe for the game

void Ball::Draw()
{
	DrawCircle((int)x, (int)y, radius, Yellow);
}

void Ball::ResetBall()
{
    x = GetScreenWidth() / 2.f;
    y = GetScreenHeight() / 2.f;
    // speed controls
    int speed_choices[2] = { -1, 1 };
    speed_x *= speed_choices[GetRandomValue(0, 1)];
    speed_y *= speed_choices[GetRandomValue(0, 1)];
}

void Ball::Update()
{
    x += speed_x;
    y += speed_y;
    //reflection from the boundary along y-axis
    if (y + radius >= GetScreenHeight() || y - radius <= 0)
    {
        speed_y *= -1;
    }
    // if ball exits from player side
    if (x + radius >= GetScreenWidth()) // Cpu wins
    {
        cpu_score++;
        ResetBall();
    }
    // if ball exits from player side
    if (x - radius <= 0) //player win
    {
        player_score++;
        ResetBall();
    }
}

void Paddle::Draw()
{
    DrawRectangleRounded(Rectangle{ x, y, width, height }, 0.8, 0, WHITE);
}
void Paddle ::LimitMovement()
{
    if (y <= 0)
    {
        y = 0;
    }
    if (y + height >= GetScreenHeight())
    {
        y = GetScreenHeight() - height;
    }
}
void Paddle::Update()
{
    if (IsKeyDown(KEY_UP))
    {
        y = y - speed;
    }
    if (IsKeyDown(KEY_DOWN))
    {
        y = y + speed;
    }
    LimitMovement();
}
void CpuPaddle::Update(int ball_y)
{
    //if y coordinate if ball is greater than y coordinate of paddle paddle will move up otherwise down
    if (y + height / 2 > ball_y)
    {
        y = y - speed;
    }
    if (y + height / 2 <= ball_y)
    {
        y = y + speed;
    }
    LimitMovement();
}
void MainMenu::launch_menu()
{
    screen = { 0,0,screen_width,screen_height };
    InitWindow(screen_width, screen_height, "Pong game!");

    InitAudioDevice();
    music = LoadMusicStream("resources/StartUP.mp3");
    PlayMusicStream(music);
    SetTargetFPS(60);
    // Printing image
    StartingImage = { 0 };
    StartingImage.format = STARTINGIMAGE_FORMAT;
    StartingImage.height = STARTINGIMAGE_HEIGHT;
    StartingImage.width = STARTINGIMAGE_WIDTH;
    StartingImage.data = STARTINGIMAGE_DATA;
    StartingImage.mipmaps = 1;
    Background = LoadTextureFromImage(StartingImage);

    while (!WindowShouldClose())
    {
        UpdateMusicStream(music);
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(Background, screen, screen, Vector2Zero(), 0, WHITE);
        DrawText(TextFormat("Press SPACE to start the game."), 200, 120, 30, WHITE);

        if (IsKeyDown(KEY_SPACE))
        {
            break;
        }
        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();
}

void gameManager::launch_game()
{
    Pong_menu.launch_menu();
    InitWindow(screen_width, screen_height, "Pong Game!");
    SetTargetFPS(60);

    ball.radius = 20;
    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width - 10;
    player.y = screen_height / 2 - player.height / 2;
    player.speed = 6;

    cpu.height = 120;
    cpu.width = 25;
    cpu.x = 10;
    cpu.y = screen_height / 2 - cpu.height / 2;
    cpu.speed = 6;
    InitAudioDevice();
    Sound HIT = LoadSound("resources/Hit.wav");
    while (WindowShouldClose() == false)
    {
        BeginDrawing(); //game loop start
        if (Paused)
        {
            DrawText("Paused", 475, screen_height / 2, 40, YELLOW);
        }
        if (IsKeyPressed(KEY_SPACE))
        {
            Paused = !Paused;
        }
        // Updating
        if (!Paused) // Play iff not paused
        {
            ball.Update();
            player.Update();
            cpu.Update(ball.y);

            // Checking for collisions
            if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ player.x, player.y, player.width, player.height }))
            {
                PlaySound(HIT);
                ball.speed_x *= -1; // move in the opposite direction
            }
            if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ cpu.x, cpu.y, cpu.width, cpu.height }))
            {
                PlaySound(HIT);
                ball.speed_x *= -1;
            }

            // Drawing
            ClearBackground(Dark_Green);

            DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Green);
            DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Green);
            DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
            ball.Draw();
            cpu.Draw();
            player.Draw();
            // CPU score display
            DrawText(TextFormat("%i", cpu_score), screen_width / 4 - 20, 20, 80, WHITE);
            // Palyer score display
            DrawText(TextFormat("%i", player_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);

        }
        EndDrawing(); //game loop ends
    }
    UnloadSound(HIT);
    CloseAudioDevice();
    CloseWindow();
}


