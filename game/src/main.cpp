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
int width = 12;
int height = 76;
typedef struct Paddle
{
    int posX; 
    float posY;
    int score;
}Paddle;

typedef struct Ball
{    
    int speedX;
    int speedY;
    float posX;
    float posY;
    float width;
    float height;
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

    ball.width = 20;
    ball.height = 20;
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

void GameDraw(Texture2D paddleTexture)
{
    BeginDrawing();
    ClearBackground(DARKGRAY);

    Vector2 leftPosition = {leftPaddle.posX, leftPaddle.posY};
    float scale = 2.0f;
    float rotation = 0.0f;
    Rectangle source = { 0, 0, paddleTexture.width, paddleTexture.height };
    Vector2 origin = { paddleTexture.width / 2.0f, paddleTexture.height / 2.0f };
    Vector2 position = { leftPaddle.posX, leftPaddle.posY };
    Rectangle dest = {
        position.x,
        position.y,
        paddleTexture.width * scale,
        paddleTexture.height * scale
    };


    DrawText(TextFormat("%i",leftPaddle.score), 10, 10, 40, PURPLE);
    DrawText(TextFormat("%i",rightPaddle.score), 1140, 10, 40, PURPLE);

    Rectangle rSource = {0,0,-paddleTexture.width, paddleTexture.height};
    Vector2 rOrigin = { paddleTexture.width / 2.0f, paddleTexture.height / 2.0f};
    Vector2 rPosition = { rightPaddle.posX, rightPaddle.posY };
    Rectangle rDest = {
        rPosition.x,
        rPosition.y,
        paddleTexture.width * scale,
        paddleTexture.height * scale
    };
    DrawTexturePro(paddleTexture, source, dest, origin, rotation, WHITE);
    //DrawRectangle(leftPaddle.posX,(int)leftPaddle.posY,width,height,PURPLE);
    //DrawRectangle(rightPaddle.posX,(int)rightPaddle.posY,width,height,PURPLE);
    //DrawRectangle(ball.posX,ball.posY,ball.width,ball.height,BLACK);
    DrawTexturePro(paddleTexture, rSource, rDest, rOrigin, rotation, WHITE);
    DrawRectangle(ball.posX, ball.posY, ball.width, ball.height, BLACK);
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

    leftPaddle.posY += leftSpeed * GetFrameTime();
    rightPaddle.posY += rightSpeed * GetFrameTime();
        
}

void Score()
{
    if(ball.posX<-ball.width)
    {
        ball.posX=GetScreenWidth()/2;
        ball.posY=GetScreenHeight()/2;
        ball.speedX=SPEEDX;
        rightPaddle.score++;
    }
    if(ball.posX+ball.width>GetScreenWidth())
    {
        ball.posX=GetScreenWidth()/2;
        ball.posY=GetScreenHeight()/2;
        ball.speedX=-SPEEDX;
        leftPaddle.score++;
    }
}

void Hit(Sound hit)
{
    //Vector2 circle = {ball.posX,ball.posY};
    Rectangle rball = {
        ball.posX,
        ball.posY,
        ball.width,
        ball.height
    };
    Rectangle leftPaddleRec ={
        leftPaddle.posX,
        leftPaddle.posY,
        width,
        height+20       
    };
    Rectangle rightPaddleRec ={
        rightPaddle.posX,
        rightPaddle.posY,
        width,
        height+20       
    };
    
    if(CheckCollisionRecs(rball,leftPaddleRec))
    {
        PlaySound(hit);
        if(ball.speedX<0)
        {
            ball.speedX = -ball.speedX;            
        }
            
        //ball.posX = leftPaddle.posX+width;        
        
    }
    if(CheckCollisionRecs(rball,rightPaddleRec))
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
    if(ball.posY<0)        
        ball.speedY *=-1;
    
    if(ball.posY+ball.height>GetScreenHeight())
        ball.speedY *=-1;

}

int main()
{
    
    GameInit();
    Sound hit = LoadSound("resources/hit.wav");
    Texture2D paddleTexture = LoadTexture("resources/paddle.png");
    SetTextureFilter(paddleTexture, TEXTURE_FILTER_BILINEAR);
    while (!WindowShouldClose())
    {
        if (!GameUpdate())
            break;
        UpdatePlayer();
        UpdateBall();
        Wall();
        Hit(hit);
        Score();
        GameDraw(paddleTexture);
    }
    UnloadTexture(paddleTexture);
    UnloadSound(hit);
    CloseAudioDevice();
    GameCleanup();

    return 0;
}