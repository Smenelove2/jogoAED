#include "Monstro.h"
#include "Objeto.h" 
#include <stdlib.h> 
#include <string.h> 

void CarregarAssetsMonstro(Monstro* m) {
    if (!m) return;

    m->sprite1 = LoadTexture("assets/esqueleto/esqueleto1.png");
    m->sprite2 = LoadTexture("assets/esqueleto/esqueleto2.png");
    m->sprite3 = LoadTexture("assets/esqueleto/esqueleto3.png");
    
}

bool IniciarMonstro(Monstro* m, 
                    Vector2 posInicial, 
                    float vida, 
                    float velocidade, 
                    float fpsAnimacao,
                    float alcanceAtaque,
                    float cooldownAtaque)
{
    if (!m) return false;

    m->posicao = posInicial;
    m->vida = vida;
    m->velocidade = velocidade;
    
    memset(&m->sprite1, 0, sizeof(Texture2D) * 3);
    
    m->fpsAnimacao = fpsAnimacao;
    m->acumulador = 0.0f;
    m->frameAtual = 1; 
    
    m->alcanceAtaque = alcanceAtaque;
    m->cooldownAtaque = cooldownAtaque;
    m->acumuladorAtaque = 0.0f;
    
    m->objeto = (struct ObjetoLancavel*)malloc(sizeof(ObjetoLancavel));
    if (!m->objeto) return false;

    if (!IniciarObjeto(m->objeto, "assets/sprites/bone.png")) {
        free(m->objeto);
        m->objeto = NULL;
        return false;
    }
    
    return true;
}

void DescarregarMonstro(Monstro* m) {
    if (!m) return;

    if (m->sprite1.id != 0) UnloadTexture(m->sprite1);
    if (m->sprite2.id != 0) UnloadTexture(m->sprite2);
    if (m->sprite3.id != 0) UnloadTexture(m->sprite3);

    if (m->objeto) {
        DescarregarObjeto(m->objeto);
        free(m->objeto);
        m->objeto = NULL;
    }
    
    memset(m, 0, sizeof(*m));
}