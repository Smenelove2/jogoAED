#include "monstro.h"
#include <stdio.h>
#include <math.h>

bool IniciarMonstro(Monstro *m, Vector2 pos, float vel, int vida, float alcance, float cooldown, const char *caminho_sprite) {
    m->posicao = pos;
    m->velocidade = vel;
    m->vida = vida;
    m->alcance_ataque = alcance;
    m->cooldown_ataque = cooldown;
    m->tempo_ataque = 0.0f; 
    
    m->sprite = LoadTexture(caminho_sprite);
    if (m->sprite.id == 0) {
        fprintf(stderr, "ERRO: Falha ao carregar sprite do monstro: %s\n", caminho_sprite);
        return false;
    }

    m->largura = m->sprite.width;
    m->altura = m->sprite.height;
    return true;
}

void AtualizarMonstro(Monstro *m, float dt, Vector2 pos_jogador) {
    
    float dist_x = pos_jogador.x - m->posicao.x;
    float dist_y = pos_jogador.y - m->posicao.y;
    float distancia = sqrtf(dist_x * dist_x + dist_y * dist_y);
    
    if (m->tempo_ataque > 0.0f) {
        m->tempo_ataque -= dt;
    }

    if (distancia > m->alcance_ataque) {
        Vector2 direcao = { dist_x / distancia, dist_y / distancia };
        
        m->posicao.x += direcao.x * m->velocidade * dt;
        m->posicao.y += direcao.y * m->velocidade * dt;
    }
}

void DesenharMonstro(Monstro *m) {
    DrawTexture(m->sprite, 
                (int)(m->posicao.x - m->largura / 2), 
                (int)(m->posicao.y - m->altura / 2), 
                WHITE);
}

void DescarregarMonstro(Monstro *m) {
    if (m->sprite.id != 0) {
        UnloadTexture(m->sprite);
    }
}

Projetil LancarProjetil(Monstro *m, Vector2 pos_alvo, Texture2D pedra_sprite, float vel_proj) {
    Vector2 direcao = { pos_alvo.x - m->posicao.x, pos_alvo.y - m->posicao.y };
    float comprimento = sqrtf(direcao.x * direcao.x + direcao.y * direcao.y);
    
    if (comprimento > 0.0f) {
        direcao.x /= comprimento;
        direcao.y /= comprimento;
    }
    
    Projetil p = {0};
    p.posicao = m->posicao;
    p.direcao = direcao;
    p.velocidade = vel_proj;
    p.ativo = true;
    p.sprite = pedra_sprite; 
    
    m->tempo_ataque = m->cooldown_ataque;

    return p;
}

void AtualizarProjetil(Projetil *p, float dt) {
    if (p->ativo) {
        p->posicao.x += p->direcao.x * p->velocidade * dt;
        p->posicao.y += p->direcao.y * p->velocidade * dt;
        
    }
}

void DesenharProjetil(Projetil *p) {
    if (p->ativo) {
        DrawTexture(p->sprite, 
                    (int)(p->posicao.x - p->sprite.width / 2), 
                    (int)(p->posicao.y - p->sprite.height / 2), 
                    WHITE);
    }
}