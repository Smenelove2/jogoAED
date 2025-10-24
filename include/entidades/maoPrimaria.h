#ifndef MAOPRIMARIA_H
#define MAOPRIMARIA_H

#include <stdbool.h>

typedef struct {
    char  nome[30];              // Nome do item
    char  descricao[200];        // Texto descritivo
    char  efeitos[100];          // Descrição resumida dos efeitos
    bool  liberado;              // Se o jogador já desbloqueou
    int   dano;                  // Dano base
    float velocidadeProjetil;    // Velocidade do projétil
    float tempoDeRecarga;        // Tempo entre usos
    float areaEfeito;            // Raio da área de impacto
    float alcance;               // Distância máxima do ataque
    int   custoMana;             // Custo de mana para usar
} MaoPrimaria;


#endif