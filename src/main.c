#include "raylib.h"
#include <math.h>

typedef struct {
    Vector2 pos;
    Vector2 vel;
    bool active;
} Bullet;

typedef struct {
    Vector2 pos;
    double spawnTime;
    bool active;
} AoE;

int main(void) {
    const int largura = 1600;
    const int altura  = 900;

    InitWindow(largura, altura, "jogoAED - Raylib: Tiros e AoE");
    SetTargetFPS(60);

    // Jogador
    Vector2 player = { largura/2.0f, altura/2.0f };
    const float playerRaio = 30.0f;
    const float playerVel = 400.0f; // pixels/segundo

    // Balas
    #define MAX_BULLETS 512
    Bullet bullets[MAX_BULLETS] = {0};
    const float bulletSpeed = 700.0f; // pixels/segundo
    const float bulletRadius = 6.0f;
    int bulletIndex = 0;

    // Áreas (AoE) de clique direito
    #define MAX_AOE 32
    AoE aoes[MAX_AOE] = {0};
    const float aoeRadius = 90.0f;
    const double aoeLifetime = 4.0; // segundos
    int aoeIndex = 0;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // ---- Update ----
        // Movimento do jogador (WASD)
        if (IsKeyDown(KEY_D)) player.x += playerVel * dt;
        if (IsKeyDown(KEY_A)) player.x -= playerVel * dt;
        if (IsKeyDown(KEY_W)) player.y -= playerVel * dt;
        if (IsKeyDown(KEY_S)) player.y += playerVel * dt;

        // Mantém o jogador dentro da janela
        if (player.x < playerRaio) player.x = playerRaio;
        if (player.x > largura - playerRaio) player.x = largura - playerRaio;
        if (player.y < playerRaio) player.y = playerRaio;
        if (player.y > altura - playerRaio) player.y = altura - playerRaio;

        // Clique esquerdo: cria um projétil na direção do mouse
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse = GetMousePosition();
            Vector2 dir = { mouse.x - player.x, mouse.y - player.y };
            float len = sqrtf(dir.x*dir.x + dir.y*dir.y);
            if (len > 0.0001f) {
                dir.x /= len;
                dir.y /= len;
                bullets[bulletIndex].pos = player; // nasce no centro do player
                bullets[bulletIndex].vel = (Vector2){ dir.x * bulletSpeed, dir.y * bulletSpeed };
                bullets[bulletIndex].active = true;
                bulletIndex = (bulletIndex + 1) % MAX_BULLETS;
            }
        }

        // Clique direito: cria uma AoE que dura 4s
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            Vector2 mouse = GetMousePosition();
            aoes[aoeIndex].pos = mouse;
            aoes[aoeIndex].spawnTime = GetTime();
            aoes[aoeIndex].active = true;
            aoeIndex = (aoeIndex + 1) % MAX_AOE;
        }

        // Atualiza balas
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (!bullets[i].active) continue;
            bullets[i].pos.x += bullets[i].vel.x * dt;
            bullets[i].pos.y += bullets[i].vel.y * dt;

            // Desativa se sair da tela (com pequena margem)
            if (bullets[i].pos.x < -50 || bullets[i].pos.x > largura + 50 ||
                bullets[i].pos.y < -50 || bullets[i].pos.y > altura + 50) {
                bullets[i].active = false;
            }
        }

        // Atualiza AoEs (desativa ao expirar)
        double now = GetTime();
        for (int i = 0; i < MAX_AOE; i++) {
            if (!aoes[i].active) continue;
            if (now - aoes[i].spawnTime >= aoeLifetime) {
                aoes[i].active = false;
            }
        }

        // ---- Draw ----
        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawText("WASD para mover. Clique ESQUERDO: tiro. Clique DIREITO: AoE 4s.", 20, 20, 20, DARKGRAY);

            // Desenhar AoEs primeiro (para ficarem sob o jogador/tiros, se quiser)
            for (int i = 0; i < MAX_AOE; i++) {
                if (!aoes[i].active) continue;
                DrawCircleV(aoes[i].pos, aoeRadius, (Color){ 255, 0, 0, 120 }); // vermelho translúcido
                DrawCircleLines((int)aoes[i].pos.x, (int)aoes[i].pos.y, aoeRadius, RED);
            }

            // Desenhar balas
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (!bullets[i].active) continue;
                DrawCircleV(bullets[i].pos, bulletRadius, RED);
            }

            // Desenhar jogador
            DrawCircleV(player, playerRaio, SKYBLUE);
            DrawCircleLines((int)player.x, (int)player.y, playerRaio, DARKBLUE);

            DrawFPS(10, altura - 30);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}