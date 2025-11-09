#ifndef MONSTRO_H
#define MONSTRO_H

#include "raylib.h"
#include <stdbool.h>

//Estrutura da pedra que ele vai jogar
typedef struct {
    Vector2 posicao;        
    Vector2 direcao;        
    float velocidade;       
    bool ativo;             
    Texture2D sprite;      
} Projetil;

//Estrutura do esqueleto
typedef struct {
    Vector2 posicao;
    float velocidade;
    int vida;
    Texture2D sprite;
    int largura;
    int altura;

    // Lógica de Ataque
    float alcance_ataque;   // Distância máxima para iniciar o ataque
    float tempo_ataque;     // Cronômetro para o cooldown entre ataques
    float cooldown_ataque;  // Tempo mínimo entre ataques (ex: 2.0s)
} Monstro;

bool IniciarMonstro(Monstro *m, Vector2 pos, float vel, int vida, float alcance, float cooldown, const char *caminho_sprite);
void AtualizarMonstro(Monstro *m, float dt, Vector2 pos_jogador);
void DesenharMonstro(Monstro *m);
void DescarregarMonstro(Monstro *m);

Projetil LancarProjetil(Monstro *m, Vector2 pos_alvo, Texture2D pedra_sprite, float vel_proj);
void AtualizarProjetil(Projetil *p, float dt);
void DesenharProjetil(Projetil *p);

#endif