#ifndef ARMADURA_H
#define ARMADURA_H

#include <stdbool.h>

typedef struct {
    char  nome[30];          // Nome da armadura
    char  descricao[200];    // Descrição detalhada
    char  efeitos[100];      // Efeitos ou habilidades especiais
    bool  liberado;          // Indica se o item está desbloqueado
    int   vidaExtra;         // Aumento de vida ao equipar
    int   armaduraExtra;     // Aumento de armadura ao equipar
} Armadura;

#endif
