#include <raylib.h>
#include <iostream>
#include <cstring>
#include <sstream>
#include <math.h>

using namespace std;

struct Projectile
{
    Vector2 pos;
    bool active;
    bool reflected;
};

//I prefer classes unless "struct is absoultely needed". New Data Type
struct AnimHead
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};


void GameInputs(int &X, int &Y, float velocity)
{
        //raylibs key press detection
        if ((IsKeyDown(KEY_W) || GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) < -0.2f) && Y > 310)
        {
            Y -= velocity * GetFrameTime();
        }

        if ((IsKeyDown(KEY_S) || GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) > 0.2f) && Y < 692)
        {
            Y += velocity * GetFrameTime();;
        }
}

void updateAnimData(AnimHead &animData, const float dt, int totalFrames)
{
    animData.runningTime += dt;
    if (animData.runningTime >= animData.updateTime)
    {
        // update animation frame
        animData.rec.x = animData.frame * animData.rec.width;
        animData.frame++;

        if (animData.frame > totalFrames)
        {
            animData.frame = 0;
        }

        animData.runningTime = 0;
    }
}

int main()
{
    const int windowHeight = 720;
    const int windowWidth = 720;

    //InitWindow function makes window appear for a split second, closes to due to function being ended.
    InitWindow(windowWidth, windowHeight, "Naruto Speed Run");

    Texture background = LoadTexture("NarutoSprites/Background/Background.png");
    Texture backgroundFlipped = LoadTexture("NarutoSprites/Background/Background_Flipped.png");

    //Player
    int playerX{windowWidth/9};
    int playerY{540};
    const float velocity{500};
    Vector2 reflectionPos;
    bool hasReflected = false;
    float reflectionCooldown{0.5f};
    float playerHealth{100};

    //Imported Naruto. Data types custom to raylib
    Texture2D naruto = LoadTexture("NarutoSprites/Naruto/NarutoRun.png"); //loading texture to variable
    Texture2D narutoHealth = LoadTexture("NarutoSprites/Naruto/NarutoHealthIcon.png"); 
    AnimHead narutoData;
    narutoData.rec.width = naruto.width/5;
    narutoData.rec.height = naruto.height;
    narutoData.rec.x = 0;
    narutoData.rec.y = 0;
    narutoData.pos.x = 0;
    narutoData.pos.y = 0;
    //animation frames
    narutoData.frame = {0};
    narutoData.updateTime = {1.0/12.0};
    narutoData.runningTime = {0};

    //Enemy
    int enemyPositionX{600};
    int enemyPositionY{540};
    bool movedUp;
    float fireTime;
    float timeToFire = 2.5;
    const float enemyVelocity{80};

    //Pain
    Texture2D pain = LoadTexture("NarutoSprites/Pain/PainStance.png"); //loading texture to variable
    AnimHead painData;
    painData.rec.width = pain.width/8;
    painData.rec.height = pain.height;
    painData.rec.x = 0;
    painData.rec.y = 0;
    painData.pos.x = 0;
    painData.pos.y = 0;
    //animation frames
    painData.frame = {0};
    painData.updateTime = {1.0/12.0};
    painData.runningTime = {0};

    //stance
    Texture2D rockStand = LoadTexture("NarutoSprites/Background/Stances.png");
    Rectangle rStand{0, 0, rockStand.width/5, rockStand.height/5};
    Vector2 rPosition{};
    float bgX;

    //Enemy Projectile
    Texture2D rock = LoadTexture("NarutoSprites/Pain/DevestationRock.png"); //loading texture to variable
    Vector2 projectilePos{};
    const float projectileVelocity{100};
    int projectileCount{0};
    Projectile projectiles[5120];

    SetTargetFPS(60);

    while((!WindowShouldClose()))
    {
        BeginDrawing();

        if (playerHealth <= 0)
            return;

        //https://www.reddit.com/r/raylib/comments/14l6x6r/is_there_a_way_to_use_hexcode_to_define_colour
        Color bgColor = GetColor(0x6B1B18ff);
        ClearBackground(bgColor);

        DrawTextureEx(background, {0,0}, 0, 1, WHITE);
        Vector2 bgPos{background.width, 0.0};
        DrawTextureEx(backgroundFlipped, bgPos, 0, 1, WHITE);
        Vector2 bg1Pos{backgroundFlipped.width*2, 0.0};
        DrawTextureEx(background, bg1Pos, 0, 1, WHITE);

        DrawTextureEx(narutoHealth, {50,50}, 0, 1.5, WHITE);
        string d = to_string(playerHealth);
        DrawText(d.c_str(), 100, 60, 30, RED);

        DrawCircle(playerX, playerY, 25, BLANK);
        DrawRectangle(enemyPositionX, enemyPositionY - 75, 70, 100, BLANK);

         if (IsGamepadAvailable(0)){
            DrawText("Controller connected!", windowWidth/2.5, windowHeight/10, 20, RED);
        }

        GameInputs(playerX, playerY, velocity);
        narutoData.pos = (Vector2){playerX - 30, playerY - 25};
        updateAnimData(narutoData, GetFrameTime(), 5);

        rPosition = (Vector2){enemyPositionX - 33, enemyPositionY - 10};
        painData.pos = (Vector2){enemyPositionX + 5, enemyPositionY - 75};
        updateAnimData(painData, GetFrameTime(), 8);

        //Reflection Input
        if (((IsKeyDown(KEY_LEFT_SHIFT)) || GetGamepadButtonPressed()) && !hasReflected)
        {
            reflectionPos = (Vector2){playerX + 25, playerY - 25};
            DrawRectangle(reflectionPos.x, reflectionPos.y, 50, 100, BLUE); //reflection
            hasReflected = true;
        }

        if (hasReflected && reflectionCooldown > 0)
        {
            reflectionCooldown -= GetFrameTime();
        } 
        else
        {
            hasReflected = false;
            reflectionCooldown = 0.5f;
        }

        if (movedUp)
        {
            enemyPositionY += enemyVelocity  * GetFrameTime();
            if (enemyPositionY >= 692)
                movedUp = false;
        } 
        else
        {
            enemyPositionY -= enemyVelocity * GetFrameTime();
            if (enemyPositionY <= 310)
                movedUp = true;
        }

        fireTime += GetFrameTime();
        if (fireTime >= timeToFire && projectileCount < 5120 && playerHealth <= 0)
        {
             fireTime = 0;
             projectiles[projectileCount].pos = (Vector2){enemyPositionX, enemyPositionY};
             projectiles[projectileCount].active = true;
             projectileCount++;
        }

        for (int i = 0; i < projectileCount; i++)
        {
           if (projectiles[i].active)
           {
              if (!projectiles[i].reflected)
              {
                 projectiles[i].pos.x -= projectileVelocity * GetFrameTime();
              } 
              else
              {
                 projectiles[i].pos.x += projectileVelocity * 2 * GetFrameTime(); //fire back direction
              }
             
              DrawCircle(projectiles[i].pos.x, projectiles[i].pos.y, 25, BLANK);
              DrawTextureEx(rock, (Vector2){projectiles[i].pos.x - 24, projectiles[i].pos.y - 24}, 0, 1.5, WHITE);

              if (projectiles[i].pos.x < 0 || projectiles[i].pos.x > windowWidth) //deactivates if offscreen
              {
                projectiles[i].active = false;
              } 

              else if (CheckCollisionCircles((Vector2){playerX, playerY}, 25, projectiles[i].pos, 25)) //deactivates when player is hit
              {
                projectiles[i].active = false;
                playerHealth -= 10;
              }
              else if (CheckCollisionCircles(reflectionPos, 40, projectiles[i].pos, 25)) //when reflection is hit, the projectile fires back
              {
                projectiles[i].reflected = true;
                reflectionPos = (Vector2){0, 0};
              } 
              else if (CheckCollisionCircles((Vector2){enemyPositionX, enemyPositionY}, 25, projectiles[i].pos, 25) && projectiles[i].reflected) //deactivates when enemy is hit
              {
                projectiles[i].active = false;
              }

           }
        }
        
        if (playerHealth <= 0)
        {
             DrawText("Game Over", windowWidth, windowHeight, 30, RED);
        } 
        else
        {
            DrawCircle(projectilePos.x, projectilePos.y, 25, RED);
            projectilePos.x -= projectileVelocity * GetFrameTime();

            DrawTextureRec(rockStand, rStand, rPosition, WHITE);
            DrawTextureRec(naruto, narutoData.rec, narutoData.pos, WHITE);
            DrawTextureRec(pain, painData.rec, painData.pos, WHITE);
        }
       

        EndDrawing();
    }

    UnloadTexture(background);
    UnloadTexture(backgroundFlipped);
    UnloadTexture(naruto);
    UnloadTexture(rock);
    UnloadTexture(pain);
    UnloadTexture(rockStand);
    UnloadTexture(narutoHealth);
    CloseWindow();
}