#ifndef LISTA_MAOPRIMARIA_H
#define LISTA_MAOPRIMARIA_H

#include <stdbool.h>
#include "maoPrimaria.h"

typedef struct NoMaoPrimaria {
    MaoPrimaria *item;
    struct NoMaoPrimaria *prox;
    struct NoMaoPrimaria *ant;
} NoMaoPrimaria;

typedef struct {
    NoMaoPrimaria *inicio;   // NULL se vazia; se não, lista circular
    int Tamanho;             // quantidade total de nós
    int EquipamentosLiberados; // quantos com liberado == true
} ListaMaoPrimaria;

#endif
