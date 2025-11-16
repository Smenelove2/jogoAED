#ifndef CAPACETE_H
#define CAPACETE_H

#include "raylib.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    // Percentuais representados como frações (0.15 = 15%)
    float reducaoTempoRecarga;
    float bonusVidaMaxima;
    float bonusRegeneracaoVida;
    float bonusDanoArmas;
} ModificadorCapacete;

typedef struct {
    const char *nome;
    const char *descricao;
    const char *spriteBase;
    ModificadorCapacete modificador;
    Texture2D sprite;
} Capacete;

extern Capacete gCapacetes[];
extern const size_t gQuantidadeCapacetes;

Capacete *ObterCapacetePorIndice(size_t indice);
Capacete *ObterCapacetePorNome(const char *nome);
bool CarregarSpriteCapacete(Capacete *cap, const char *diretorioSprites);
void DescarregarSpriteCapacete(Capacete *cap);
void DesenharCapacete(const Capacete *cap, Vector2 posicaoCentroJogador, float escala);
const char *GerarNomeSpriteCapacete(const char *nome, char *buffer, int tamanhoBuffer);

#endif // CAPACETE_H
