#ifndef MONSTRO_H
#define MONSTRO_H

#include "raylib.h"
#include <stdbool.h>

struct ObjetoLancavel; 
struct Jogador;

typedef struct Monstro {
    Vector2 posicao;
    float vida;
    float velocidade;
    
    Texture2D sprite1;
    Texture2D sprite2;
    Texture2D sprite3;

    float fpsAnimacao;
    float acumulador;
    int frameAtual; 

    float alcanceAtaque;
    float cooldownAtaque;
    float acumuladorAtaque;
    
    struct ObjetoLancavel *objeto; 
} Monstro;

bool IniciarMonstro(Monstro* m, 
                    Vector2 posInicial, 
                    float vida, 
                    float velocidade, 
                    float fpsAnimacao,
                    float alcanceAtaque,
                    float cooldownAtaque);

void DescarregarMonstro(Monstro* m);

void CarregarAssetsMonstro(Monstro* m); 

Vector2 GerarMonstros(struct Jogador *jogador,int mapL, int mapC,int tileW, int tileH);

#endif 