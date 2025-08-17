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
float width = 14.0f;
float height = 80.0f;
Sound hit; // Declare a global Sound variable
typedef struct Paddle
{
    int posX; 
    float posY;
    int score;
}Paddle;
int leftScore;
int rightScore;

Rectangle ball, leftPaddle, rightPaddle;
float ballSpeedX = SPEEDX;
float ballSpeedY = SPEEDY;
// Paddle leftPaddle,rightPaddle;
// Ball ball;
void GameInit()
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(InitialWidth, InitialHeight, "Example");
    InitAudioDevice();
    
    SetTargetFPS(144);
    
    leftScore = 0;
    rightScore = 0;
    leftPaddle = {0,400,width,200};
    rightPaddle = {GetScreenWidth()-width,400,width,200};    
    ball = {600,400,20,20,};
    
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

void GameDraw(Texture2D paddleTexture)
{
    BeginDrawing();
    ClearBackground(DARKGRAY);

    Vector2 leftPosition = {leftPaddle.x, leftPaddle.y};
    float scale = 2.0f;
    float rotation = 0.0f;
    Rectangle source = { 0, 0, paddleTexture.width, paddleTexture.height };
    Vector2 origin = { paddleTexture.width / 2.0f, paddleTexture.height / 2.0f };
    Vector2 position = { leftPaddle.x, leftPaddle.y };
    Rectangle dest = {
        position.x,
        position.y,
        paddleTexture.width * scale,
        paddleTexture.height * scale
    };


    DrawText(TextFormat("%i",leftScore), 10, 10, 40, PURPLE);
    DrawText(TextFormat("%i",rightScore), 1140, 10, 40, PURPLE);

    Rectangle rSource = {0,0,-paddleTexture.width, paddleTexture.height};
    Vector2 rOrigin = { paddleTexture.width / 2.0f, paddleTexture.height / 2.0f};
    Vector2 rPosition = { rightPaddle.x, rightPaddle.y };
    Rectangle rDest = {
        rPosition.x,
        rPosition.y,
        paddleTexture.width * scale,
        paddleTexture.height * scale
    };
    //DrawTexturePro(paddleTexture, source, dest, origin, rotation, WHITE);
    DrawRectangle(leftPaddle.x,leftPaddle.y,leftPaddle.width,leftPaddle.height,PURPLE);
    DrawRectangle(rightPaddle.x,rightPaddle.y,rightPaddle.width,rightPaddle.height,PURPLE);
    //DrawRectangle(ball.posX,ball.posY,ball.width,ball.height,BLACK);
    //DrawTexturePro(paddleTexture, rSource, rDest, rOrigin, rotation, WHITE);
    DrawRectangle(ball.x, ball.y, ball.width, ball.height, BLACK);
    EndDrawing();
}

void UpdatePlayer()
{
    float speed=400.0f;
    float leftSpeed = 0.0f;
    float rightSpeed = 0.0f;
    float paddleAcceleration = 700.0f;
    float maxSpeed = 200000.0f;
    float friction = 200.0f;

    if(IsKeyDown(KEY_W))
    {
        leftSpeed -= paddleAcceleration;
        if(leftSpeed<-maxSpeed) leftSpeed = -maxSpeed;
        //leftPaddle.posY -= GetFrameTime()*speed;
    }
    else if(IsKeyDown(KEY_S))
    {
        leftSpeed += paddleAcceleration;
        if(leftSpeed>maxSpeed) leftSpeed = maxSpeed;
        //leftPaddle.posY += GetFrameTime()*speed;
    }
    else
    {
        if(leftSpeed>0){
            leftSpeed -= friction*GetFrameTime();
            if(leftSpeed<0) leftSpeed=0;
        }
        else if(leftSpeed<0){
            leftSpeed += friction*GetFrameTime();
            if(leftSpeed>0) leftSpeed=0;
        }
    }

    if(IsKeyDown(KEY_UP))
    {
        rightSpeed -= paddleAcceleration;
        if(rightSpeed<-maxSpeed) rightSpeed = -maxSpeed;
    }        
    else if(IsKeyDown(KEY_DOWN))
    {
        rightSpeed += paddleAcceleration;
        if(rightSpeed>maxSpeed) rightSpeed = maxSpeed;
    }
    else
    {
        if(rightSpeed>0){
            rightSpeed -= friction*GetFrameTime();
            if(rightSpeed<0) rightSpeed=0;
        }
        else if(rightSpeed<0){
            rightSpeed += friction*GetFrameTime();
            if(rightSpeed>0) rightSpeed=0;
        }
    }

    leftPaddle.y += leftSpeed * GetFrameTime();
    rightPaddle.y += rightSpeed * GetFrameTime();
        
}

// void Score()
// {
//     if(ball.x<-ball.width)
//     {
//         ball.x=GetScreenWidth()/2;
//         ball.posY=GetScreenHeight()/2;
//         ball.speedX=SPEEDX;
//         rightPaddle.score++;
//     }
//     if(ball.x+ball.width>GetScreenWidth())
//     {
//         ball.x=GetScreenWidth()/2;
//         ball.posY=GetScreenHeight()/2;
//         ball.speedX=-SPEEDX;
//         leftPaddle.score++;
//     }
// }

// void Hit(Sound hit)
// {
//     //Vector2 circle = {ball.x,ball.posY};
//     Rectangle rball = {
//         ball.x,
//         ball.posY,
//         ball.width,
//         ball.height
//     };
//     Rectangle leftPaddleRec ={
//         leftPaddle.posX,
//         leftPaddle.posY-10,
//         width,
//         height+20       
//     };
//     Rectangle rightPaddleRec ={
//         rightPaddle.posX,
//         rightPaddle.posY- 10,
//         width,
//         height+20       
//     };
    
//     if(CheckCollisionRecs(rball,leftPaddleRec))
//     {
//         PlaySound(hit);
//         if(ball.speedX<0)
//         {
//             ball.speedX = -ball.speedX;            
//         }
            
//         //ball.x = leftPaddle.posX+width;        
        
//     }
//     if(CheckCollisionRecs(rball,rightPaddleRec))
//     {
//         PlaySound(hit);
//         if(ball.speedX>0)
//         {
//             ball.speedX = -ball.speedX *1.03;
//             PlaySound(hit);
//         }
//         //ball.x = rightPaddle.posX-width;
//     }    
    
// }

void UpdateBall()
{
    bool hitPaddle = false;

    if((CheckCollisionRecs(ball,leftPaddle) && ball.x<leftPaddle.x+leftPaddle.width) || (CheckCollisionRecs(ball,rightPaddle) && ball.x+ball.width<rightPaddle.x+rightPaddle.width))
    {
        PlaySound(hit);        
        ballSpeedX = -ballSpeedX;
        hitPaddle = true; 
        //ball.x = leftPaddle.posX+width;                
    }
    else if (ball.y < 0 || ball.y + ball.height > GetScreenHeight())
        ballSpeedY = -ballSpeedY;
    if(!hitPaddle)
    {
        if(ball.x<-ball.width)
        {
            ball.x=GetScreenWidth()/2;
            ball.y=GetScreenHeight()/2;
            ballSpeedX=SPEEDX;
            rightScore++;
        }
        if(ball.x+ball.width>GetScreenWidth())
        {
            ball.x=GetScreenWidth()/2;
            ball.y=GetScreenHeight()/2;
            ballSpeedX=-SPEEDX;
            leftScore++;
        }
    }
    
    ball.x += GetFrameTime()*ballSpeedX;
    ball.y += GetFrameTime()*ballSpeedY;
    
}

void Wall()
{
    if(ball.y<0)        
        ballSpeedY = -ballSpeedX;
    
    if(ball.y+ball.height>GetScreenHeight())
        ballSpeedY = -ballSpeedY;

}

int main()
{
    
    GameInit();
    hit = LoadSound("resources/hit.wav");
    Texture2D paddleTexture = LoadTexture("resources/paddle.png");
    SetTextureFilter(paddleTexture, TEXTURE_FILTER_BILINEAR);
    while (!WindowShouldClose())
    {
        if (!GameUpdate())
            break;
        UpdatePlayer();
        UpdateBall();
        //Wall();
        //Hit(hit);
        //Score();
        GameDraw(paddleTexture);
    }
    UnloadTexture(paddleTexture);
    UnloadSound(hit);
    CloseAudioDevice();
    GameCleanup();

    return 0;
}