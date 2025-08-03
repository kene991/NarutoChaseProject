#include <raylib.h>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

void BackgroundSetup(Texture texture, Texture flip)
{
        //https://www.reddit.com/r/raylib/comments/14l6x6r/is_there_a_way_to_use_hexcode_to_define_colour
        Color bgColor = GetColor(0x6B1B18ff);
        ClearBackground(bgColor);

        DrawTextureEx(texture, {0,0}, 0, 1, WHITE);
        Vector2 bgPos{texture.width, 0.0};
        DrawTextureEx(flip, bgPos, 0, 1, WHITE);
        Vector2 bg1Pos{texture.width*2, 0.0};
        DrawTextureEx(texture, bg1Pos, 0, 1, WHITE);
}

void GameInputs(int &X, int &Y, float velocity)
{
        //raylibs key press detection
        if ((IsKeyDown(KEY_W) || GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) < -0.2f) && Y > 310)
        {
            Y -= velocity;
        }

        if ((IsKeyDown(KEY_A) || GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) < -0.1f) && X > 25)
        {
            X -= velocity;
        }

        if ((IsKeyDown(KEY_S) || GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) > 0.2f) && Y < 692)
        {
            Y += velocity;
        }

        if ((IsKeyDown(KEY_D) || GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) > 0.2f) && X < 1000)
        {
            X += velocity;
        }
}

int main()
{

    const int windowHeight = 720;
    const int windowWidth = 1280;

    //InitWindow function makes window appear for a split second, closes to due to function being ended.
    InitWindow(windowWidth, windowHeight, "Naruto Speed Run");

    Texture background = LoadTexture("NarutoSprites/Background/Background.png");
    Texture backgroundFlipped = LoadTexture("NarutoSprites/Background/Background_Flipped.png");

    //Player (circle) coordinates
    int circleX{300};
    int circleY{540};
    const float velocity{2.5};

    //Player (circle) coordinates
    //int circleX{300};
    //int circleY{540};

    SetTargetFPS(60);

    while((!WindowShouldClose()))
    {
        BeginDrawing();

        BackgroundSetup(background, backgroundFlipped);

        DrawCircle(circleX, circleY, 25, GREEN);

         if (IsGamepadAvailable(0)){
            DrawText("Controller connected!", windowWidth/2.5, windowHeight/10, 20, RED);
        }

        GameInputs(circleX, circleY, velocity);

        EndDrawing();
    }

    UnloadTexture(background);
    UnloadTexture(backgroundFlipped);
    CloseWindow();
}