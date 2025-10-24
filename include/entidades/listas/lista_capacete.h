#ifndef LISTA_CAPACETE_H
#define LISTA_CAPACETE_H

#include <stdbool.h>
#include "capacete.h"

typedef struct NoCapacete {
    Capacete *item;
    struct NoCapacete *prox;
    struct NoCapacete *ant;
} NoCapacete;

typedef struct {
    NoCapacete *inicio;
    int Tamanho;
    int EquipamentosLiberados;
} ListaCapacete;

#endif
