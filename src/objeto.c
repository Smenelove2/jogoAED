#include "objeto.h"
#include <string.h>

bool IniciarObjeto(ObjetoLancavel* o, const char* caminhoSprite) {
    if (!o) return false;
    
    o->sprite = LoadTexture(caminhoSprite);
    if (o->sprite.id == 0) {
        memset(o, 0, sizeof(*o));
        return true; 
    }

    o->posicao = (Vector2){0, 0};
    o->direcao = (Vector2){0, 0};
    o->velocidade = 800.0f; 
    o->dano = 10.0f;
    o->ativo = false;
    
    return true;
}

void DescarregarObjeto(ObjetoLancavel* o) {
    if (!o) return;
    if (o->sprite.id != 0) UnloadTexture(o->sprite);
    memset(o, 0, sizeof(*o));
}