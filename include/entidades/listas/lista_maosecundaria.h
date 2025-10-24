#ifndef LISTA_MAOSECUNDARIA_H
#define LISTA_MAOSECUNDARIA_H

#include <stdbool.h>
#include "maoSecundaria.h"

typedef struct NoMaoSecundaria {
    MaoSecundaria *item;
    struct NoMaoSecundaria *prox;
    struct NoMaoSecundaria *ant;
} NoMaoSecundaria;

typedef struct {
    NoMaoSecundaria *inicio;
    int Tamanho;
    int EquipamentosLiberados;
} ListaMaoSecundaria;

#endif
