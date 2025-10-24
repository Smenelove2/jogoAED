#ifndef CAPACETE_H
#define CAPACETE_H

#include <stdbool.h>

typedef struct {
    char  nome[30];          // Nome do capacete
    char  descricao[200];    // Descrição detalhada
    char  efeitos[100];      // Efeitos ou habilidades especiais
    bool  liberado;          // Indica se o item está desbloqueado
    int   vidaExtra;         // Aumento de vida ao equipar
    int   armaduraExtra;     // Aumento de armadura ao equipar
} Capacete;

#endif
