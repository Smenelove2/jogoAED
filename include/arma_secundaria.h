#ifndef ARMA_SECUNDARIA_H
#define ARMA_SECUNDARIA_H

#include "raylib.h"
#include <stdbool.h>
#include <stddef.h>

typedef enum {
    TIPO_ARMA_SECUNDARIA_AREA_PULSANTE = 0,
    TIPO_ARMA_SECUNDARIA_CONE_EMPURRAO,
    TIPO_ARMA_SECUNDARIA_DEFESA_TEMPORARIA,
    TIPO_ARMA_SECUNDARIA_ZONA_LENTIDAO
} TipoArmaSecundaria;

typedef struct {
    const char *nome;
    const char *descricao;
    const char *status;
    TipoArmaSecundaria tipo;
    float dano;
    float raioOuAlcance;
    float duracao;
    float tempoRecarga;
    const char *spriteBase;
    Texture2D spriteIdleOuFrame1;
    Texture2D spriteFrame2;
    Vector2 offsetFrame1;
    Vector2 offsetFrame2;
} ArmaSecundaria;

extern ArmaSecundaria gArmasSecundarias[];
extern const size_t gQuantidadeArmasSecundarias;

const ArmaSecundaria *ObterArmaSecundariaPorIndice(size_t indice);
const ArmaSecundaria *ObterArmaSecundariaPorNome(const char *nome);
bool CarregarSpritesArmaSecundaria(ArmaSecundaria *hab, const char *diretorio);
void DescarregarSpritesArmaSecundaria(ArmaSecundaria *hab);
void DesenharArmaSecundaria(const ArmaSecundaria *hab, Vector2 posicaoCentro,
                                  bool emMovimento, bool frameAlternado, float escala);
const char *GerarNomeSpriteArmaSecundaria(const char *nome, char *buffer, int tamanhoBuffer);

#endif // ARMA_SECUNDARIA_H
