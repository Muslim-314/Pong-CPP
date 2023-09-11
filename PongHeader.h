#pragma once
#include <iostream>
#include <raylib.h>
#include"raymath.h"



class Ball
{
public:
    float x, y; // coordinates
    float speed_x, speed_y; // speed along x,y
    float radius;
    void Draw();
    void Update();
    void ResetBall();
};

class Paddle
{
protected:
    void LimitMovement();
public:
    float x, y;
    float width, height;
    int speed;
    void Draw();
    void Update();
};

class CpuPaddle : public Paddle // paddle controlled by the computer
{
public:
    void Update(int ball_y);
    
};

class MainMenu {
    Texture2D Background;
    Rectangle screen;
    Music music;
    Image StartingImage;
public:
    void launch_menu();
};

class gameManager
{
private:
    Ball ball;
    Paddle player;
    CpuPaddle cpu;
    MainMenu Pong_menu;
public:
    void launch_game();
};