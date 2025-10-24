#ifndef PERSONAGEM_H
#define PERSONAGEM_H

#include "maoPrimaria.h"
#include "maoSecundaria.h"
#include "armadura.h"
#include "capacete.h"

typedef struct {
    char nome[50];
    int vida;
    int ataque;
    int armadura;
    float velocidade;

    MaoPrimaria   *maoPrimaria;
    MaoSecundaria *maoSecundaria;
    Armadura      *armadura;
    Capacete      *capacete;
} Personagem;


#endif