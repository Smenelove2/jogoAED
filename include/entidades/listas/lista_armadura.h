#ifndef LISTA_ARMADURA_H
#define LISTA_ARMADURA_H

#include <stdbool.h>
#include "armadura.h"

typedef struct NoArmadura {
    Armadura *item;
    struct NoArmadura *prox;
    struct NoArmadura *ant;
} NoArmadura;

typedef struct {
    NoArmadura *inicio;
    int Tamanho;
    int EquipamentosLiberados;
} ListaArmadura;

#endif
