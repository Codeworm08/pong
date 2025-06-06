/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

-- Copyright (c) 2020-2024 Jeffery Myers
--
--This software is provided "as-is", without any express or implied warranty. In no event 
--will the authors be held liable for any damages arising from the use of this software.

--Permission is granted to anyone to use this software for any purpose, including commercial 
--applications, and to alter it and redistribute it freely, subject to the following restrictions:

--  1. The origin of this software must not be misrepresented; you must not claim that you 
--  wrote the original software. If you use this software in a product, an acknowledgment 
--  in the product documentation would be appreciated but is not required.
--
--  2. Altered source versions must be plainly marked as such, and must not be misrepresented
--  as being the original software.
--
--  3. This notice may not be removed or altered from any source distribution.

*/

#include "raylib.h"
#include "raymath.h"

#include "game.h"   // an external header in this project
#include "lib.h"	// an external header in the static lib project

#define SPEEDX 500
#define SPEEDY 300
int width = 40;
int height = 100;
typedef struct Paddle
{
    int posX; 
    float posY;
    int score;
}Paddle;

typedef struct Ball
{
    float radius;
    int speedX;
    int speedY;
    float posX;
    float posY;        
}Ball;

Paddle leftPaddle,rightPaddle;
Ball ball;
void GameInit()
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(InitialWidth, InitialHeight, "Example");
    InitAudioDevice();
    
    SetTargetFPS(144);

    // load resources
     
    leftPaddle.posX=0;
    leftPaddle.posY=400-(int)(height/2);
    leftPaddle.score=0;
    

    rightPaddle.posX=1200-width;
    rightPaddle.posY=400-(int)(height/2);
    rightPaddle.score=0;

    ball.radius = 10.0f;
    ball.posX = 600;
    ball.posY = 400;
    ball.speedX = SPEEDX;
    ball.speedY = SPEEDY;
}

void GameCleanup()
{
    // unload resources

    CloseWindow();
}

bool GameUpdate()
{
    return true;
}

void GameDraw()
{
    BeginDrawing();
    ClearBackground(DARKGRAY);

    DrawText(TextFormat("%i",leftPaddle.score), 10, 10, 40, PURPLE);
    DrawText(TextFormat("%i",rightPaddle.score), 1140, 10, 40, PURPLE);

    DrawRectangle(leftPaddle.posX,(int)leftPaddle.posY,width,height,PURPLE);
    DrawRectangle(rightPaddle.posX,(int)rightPaddle.posY,width,height,PURPLE);
    DrawCircle((int)ball.posX,(int)ball.posY,ball.radius,BLACK);
    EndDrawing();
}

void UpdatePlayer()
{
    float speed=400.0f;
    if(IsKeyDown(KEY_W))
        leftPaddle.posY -= GetFrameTime()*speed;
    if(IsKeyDown(KEY_S))
        leftPaddle.posY += GetFrameTime()*speed;
    if(IsKeyDown(KEY_UP))
        rightPaddle.posY -= GetFrameTime()*speed;
    if(IsKeyDown(KEY_DOWN))
        rightPaddle.posY += GetFrameTime()*speed;
}

void Score()
{
    if(ball.posX<0)
    {
        ball.posX=width+10;
        ball.speedX=SPEEDX;
        rightPaddle.score++;
    }
    if(ball.posX>GetScreenWidth()-width)
    {
        ball.posX=1200-width-10;
        ball.speedX=-SPEEDX;
        leftPaddle.score++;
    }
}

void Hit(Sound hit)
{
    Vector2 circle = {ball.posX,ball.posY};
    Rectangle leftPaddleRec ={
        leftPaddle.posX,
        leftPaddle.posY,
        width,
        height        
    };
    Rectangle rightPaddleRec ={
        rightPaddle.posX,
        rightPaddle.posY,
        width,
        height        
    };
    
    if(CheckCollisionCircleRec(circle,ball.radius,leftPaddleRec))
    {
        PlaySound(hit);
        if(ball.speedX<0)
        {
            ball.speedX = -ball.speedX;            
        }
            
        //ball.posX = leftPaddle.posX+width;        
        
    }
    if(CheckCollisionCircleRec(circle,ball.radius,rightPaddleRec))
    {
        PlaySound(hit);
        if(ball.speedX>0)
        {
            ball.speedX = -ball.speedX *1.03;
            PlaySound(hit);
        }
        //ball.posX = rightPaddle.posX-width;
    }    
    
}

void UpdateBall()
{
    ball.posX += GetFrameTime()*ball.speedX;
    ball.posY += GetFrameTime()*ball.speedY;
    
}

void Wall()
{
    if(ball.posY-ball.radius<0)        
        ball.speedY *=-1;
    
    if(ball.posY+ball.radius>800)
        ball.speedY *=-1;

}

int main()
{
    
    GameInit();
    Sound hit = LoadSound("resources/hit.wav");
    while (!WindowShouldClose())
    {
        if (!GameUpdate())
            break;
        UpdatePlayer();
        UpdateBall();
        Wall();
        Hit(hit);
        Score();
        GameDraw();
    }
    UnloadSound(hit);
    CloseAudioDevice();
    GameCleanup();

    return 0;
}