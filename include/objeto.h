#ifndef OBJETO_H
#define OBJETO_H

#include "raylib.h"
#include <stdbool.h>

typedef struct ObjetoLancavel {
    Vector2 posicao;
    Vector2 direcao;
    float velocidade;
    float dano;
    bool ativo; 
    
    Texture2D sprite; 
} ObjetoLancavel;

bool IniciarObjeto(ObjetoLancavel* o, const char* caminhoSprite);
void DescarregarObjeto(ObjetoLancavel* o);

#endif 