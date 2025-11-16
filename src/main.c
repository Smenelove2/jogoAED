#include "raylib.h"
#include "jogador.h"
#include "mapa.h"
#include "capacete.h"
#include "armadura.h"
#include "arma_principal.h"
#include "arma_secundaria.h"
#include "equipamentos.h"
#include "ui_utils.h"
#include "menu.h"
#include "jogo.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define MAP_L 65
#define MAP_C 65

static Texture2D gTiles[14];
static int TILE_W = 64;
static int TILE_H = 64;
static Mapa **gMapa = NULL;

typedef enum {
    TELA_MENU = 0,
    TELA_CONFIG,
    TELA_JOGO
} TelaAtual;

static TelaAtual gTelaAtual = TELA_MENU;
static float gVidaBaseJogador = 100.0f;
static const float LARGURA_BASE_UI = 1280.0f;
static const float ALTURA_BASE_UI = 720.0f;

int main(void)
{
    const int larguraInicial = 1280;
    const int alturaInicial = 720;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(larguraInicial, alturaInicial, "Tarefa: Identificar No do Jogador");
    SetWindowMinSize(960, 540);
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    Font fonteNormal = LoadFont("assets/fontes/PixelOperator.ttf");
    Font fonteBold = LoadFont("assets/fontes/PixelOperator-Bold.ttf");
    Font fonteBoldPequena = LoadFont("assets/fontes/PixelOperator8-Bold.ttf");

    gTiles[12] = LoadTexture("assets/tiles/grama1.png");
    if (gTiles[12].id == 0) {
        printf("Erro: Nao foi possivel carregar tile de referencia 'grama1.png'\n");
        CloseWindow();
        return 1;
    }
    TILE_W = gTiles[12].width;
    TILE_H = gTiles[12].height;
    printf("Tamanho do tile detectado: %dx%d\n", TILE_W, TILE_H);

    gMapa = criar_mapa_encadeado(MAP_L, MAP_C, TILE_W, TILE_H);
    if (!gMapa) {
        UnloadTexture(gTiles[12]);
        CloseWindow();
        return 1;
    }

    gTiles[0]  = LoadTexture("assets/tiles/cercadoPonta1.png");
    gTiles[1]  = LoadTexture("assets/tiles/cercadoPonta2.png");
    gTiles[2]  = LoadTexture("assets/tiles/cercadoPonta3.png");
    gTiles[3]  = LoadTexture("assets/tiles/cercadoPonta4.png");
    gTiles[4]  = LoadTexture("assets/tiles/cercadoEsquerda1.png");
    gTiles[5]  = LoadTexture("assets/tiles/cercadoEsquerda2.png");
    gTiles[6]  = LoadTexture("assets/tiles/cercadoDireita1.png");
    gTiles[7]  = LoadTexture("assets/tiles/cercadoDireita2.png");
    gTiles[8]  = LoadTexture("assets/tiles/cercadoCima1.png");
    gTiles[9]  = LoadTexture("assets/tiles/cercadoCima2.png");
    gTiles[10] = LoadTexture("assets/tiles/cercadoBaixo1.png");
    gTiles[11] = LoadTexture("assets/tiles/cercadoBaixo2.png");
    gTiles[13] = LoadTexture("assets/tiles/grama2.png");

    bool okTiles = true;
    for (int t = 0; t < 14; ++t) {
        if (gTiles[t].id == 0) {
            printf("Erro: Falha ao carregar tile ID %d\n", t);
            okTiles = false;
        }
    }
    if (!okTiles) {
        for (int t = 0; t < 14; ++t) {
            if (gTiles[t].id != 0) UnloadTexture(gTiles[t]);
        }
        destruir_mapa_encadeado(gMapa, MAP_L);
        CloseWindow();
        return 1;
    }

    Vector2 posInicial = {
        (MAP_C * TILE_W) / 2.0f,
        (MAP_L * TILE_H) / 2.0f
    };

    Jogador jogador;
    bool ok = IniciarJogador(
        &jogador,
        posInicial,
        400.0f,
        10.0f,
        100.0f,
        "assets/personagem/personagemParado.png",
        "assets/personagem/personagemAndando1.png",
        "assets/personagem/personagemAndando2.png"
    );
    if (!ok) {
        for (int t = 0; t < 14; ++t) UnloadTexture(gTiles[t]);
        destruir_mapa_encadeado(gMapa, MAP_L);
        CloseWindow();
        return 1;
    }
    gVidaBaseJogador = jogador.vidaMaxima;

    CarregarTexturasEquipamentos();

    EstadoMenu estadoMenu;
    MenuInicializarEstado(&estadoMenu);
    EstadoJogo estadoJogo;
    JogoInicializar(&estadoJogo, jogador.regeneracaoBase);

    Capacete *capaceteAtual = NULL;
    Armadura *armaduraAtual = NULL;
    ArmaPrincipal *armaPrincipalAtual = NULL;
    ArmaSecundaria *armaSecundariaAtual = NULL;

    AtualizarNoAtualJogador(&jogador, gMapa, MAP_L, MAP_C, TILE_W, TILE_H);
    if (jogador.noAtual) {
        printf("Jogador iniciou no no (%d, %d)\n", jogador.noAtual->linha, jogador.noAtual->coluna);
    }

    Camera2D camera = {0};
    camera.target = jogador.posicao;
    camera.offset = (Vector2){ larguraInicial / 2.0f, alturaInicial / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    bool solicitarEncerramento = false;
    while (!solicitarEncerramento) {
        if (WindowShouldClose()) break;

        float dt = GetFrameTime();
        int largura = GetScreenWidth();
        int altura = GetScreenHeight();
        float escalaH = (float)largura / LARGURA_BASE_UI;
        float escalaV = (float)altura / ALTURA_BASE_UI;
        float escalaUIAtual = fminf(escalaH, escalaV);
        if (escalaUIAtual < 0.7f) escalaUIAtual = 0.7f;
        if (escalaUIAtual > 1.8f) escalaUIAtual = 1.8f;
        UI_SetEscala(escalaUIAtual);

        camera.offset = (Vector2){ largura / 2.0f, altura / 2.0f };
        Vector2 mousePos = GetMousePosition();
        bool mouseCliqueEsq = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        bool mouseCliqueDir = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);
        bool escapePress = IsKeyPressed(KEY_ESCAPE);

        BeginDrawing();
        ClearBackground((Color){12, 12, 26, 255});

        switch (gTelaAtual) {
            case TELA_MENU: {
                AcaoMenu acao = MenuDesenharTelaInicial(&estadoMenu,
                                                        mousePos,
                                                        mouseCliqueEsq,
                                                        fonteBold,
                                                        fonteBoldPequena,
                                                        largura,
                                                        altura);
                if (acao == MENU_ACAO_JOGAR) {
                    gTelaAtual = TELA_CONFIG;
                } else if (acao == MENU_ACAO_SAIR) {
                    solicitarEncerramento = true;
                }
            } break;
            case TELA_CONFIG: {
                ResultadoMenu resultado = MenuDesenharTelaConfig(&estadoMenu,
                                                                 mousePos,
                                                                 mouseCliqueEsq,
                                                                 fonteNormal,
                                                                 fonteBold,
                                                                 &jogador,
                                                                 largura,
                                                                 altura);
                if (resultado.acao == MENU_ACAO_VOLTAR) {
                    gTelaAtual = TELA_MENU;
                } else if (resultado.acao == MENU_ACAO_JOGAR) {
                    capaceteAtual = resultado.capacete;
                    armaduraAtual = resultado.armadura;
                    armaPrincipalAtual = resultado.armaPrincipal;
                    armaSecundariaAtual = resultado.armaSecundaria;
                    if (armaPrincipalAtual) armaPrincipalAtual->tempoRecargaRestante = 0.0f;
                    estadoJogo.regeneracaoAtual =
                        AtualizarVidaJogadorComEquipamentos(&jogador, armaduraAtual, capaceteAtual, gVidaBaseJogador);
                    jogador.vida = jogador.vidaMaxima;
                    JogoReiniciar(&estadoJogo, &jogador, &camera, posInicial);
                    gTelaAtual = TELA_JOGO;
                }
            } break;
            case TELA_JOGO: {
                Vector2 mouseNoMundo = GetScreenToWorld2D(mousePos, camera);
                JogoAtualizar(&estadoJogo,
                              &jogador,
                              &camera,
                              gMapa,
                              MAP_L,
                              MAP_C,
                              TILE_W,
                              TILE_H,
                              dt,
                              mouseNoMundo,
                              mouseCliqueEsq,
                              mouseCliqueDir,
                              escapePress,
                              armaduraAtual,
                              capaceteAtual,
                              armaPrincipalAtual,
                              armaSecundariaAtual);
                JogoDesenhar(&estadoJogo,
                             &jogador,
                             &camera,
                             gMapa,
                             gTiles,
                             14,
                             MAP_L,
                             MAP_C,
                             TILE_W,
                             TILE_H,
                             largura,
                             altura,
                             fonteNormal,
                             fonteBold,
                             armaduraAtual,
                             capaceteAtual,
                             armaPrincipalAtual,
                             armaSecundariaAtual,
                             mousePos,
                             mouseCliqueEsq);
                if (estadoJogo.solicitouRetornoMenu) {
                    estadoJogo.solicitouRetornoMenu = false;
                    gTelaAtual = TELA_MENU;
                }
            } break;
        }

        EndDrawing();

        if (solicitarEncerramento) break;
    }

    JogoLiberarRecursos(&estadoJogo);
    DescarregarTexturasEquipamentos();
    DescarregarJogador(&jogador);
    for (int t = 0; t < 14; ++t) {
        if (gTiles[t].id != 0) {
            UnloadTexture(gTiles[t]);
        }
    }
    destruir_mapa_encadeado(gMapa, MAP_L);
    UnloadFont(fonteNormal);
    UnloadFont(fonteBold);
    UnloadFont(fonteBoldPequena);
    CloseWindow();
    return 0;
}
