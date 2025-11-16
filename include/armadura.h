#ifndef ARMADURA_H
#define ARMADURA_H

#include "raylib.h"
#include <stdbool.h>
#include <stddef.h>

typedef enum {
    STATUS_VIDA_MAXIMA = 0,
    STATUS_VELOCIDADE,
    STATUS_DANO
} TipoStatusArmadura;

typedef struct {
    TipoStatusArmadura statusBeneficio;
    int valorBeneficio;
    TipoStatusArmadura statusPenalidade;
    int valorPenalidade;
} ModificadorArmadura;

typedef struct {
    const char *nome;
    const char *descricao;
    const char *spriteBase;
    ModificadorArmadura modificador;
    Texture2D spriteIdle;
    Texture2D spriteAndando1;
    Texture2D spriteAndando2;
} Armadura;

extern Armadura gArmaduras[];
extern const size_t gQuantidadeArmaduras;

Armadura *ObterArmaduraPorIndice(size_t indice);
Armadura *ObterArmaduraPorNome(const char *nome);
bool CarregarSpritesArmadura(Armadura *arm, const char *diretorioSprites);
void DescarregarSpritesArmadura(Armadura *arm);
void DesenharArmadura(const Armadura *arm, Vector2 posicaoCentroJogador,
                      bool emMovimento, bool frameAlternado, float escala);
const char *GerarNomeSpriteArmadura(const char *nome, char *buffer, int tamanhoBuffer);

#endif // ARMADURA_H
