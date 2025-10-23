#include "raylib.h"

int main(void) {
    const int comprimentoTela = 800;
    const int alturaTela = 450;

    InitWindow(comprimentoTela, alturaTela, "jogoAED - Raylib template");
    SetTargetFPS(60);

    Vector2 pos = { comprimentoTela/2.0f, alturaTela/2.0f };
    float raio = 30.0f;

    while (!WindowShouldClose()) {
        // Update
        if (IsKeyDown(KEY_RIGHT)) pos.x += 4;
        if (IsKeyDown(KEY_LEFT))  pos.x -= 4;
        if (IsKeyDown(KEY_UP))    pos.y -= 4;
        if (IsKeyDown(KEY_DOWN))  pos.y += 4;

        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Testando Raylib! Use as setas.", 20, 20, 20, DARKGRAY);
            DrawCircleV(pos, raio, SKYBLUE);
            DrawFPS(10, alturaTela - 30);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
