#ifndef UI_UTILS_H
#define UI_UTILS_H

#include "raylib.h"
#include <stdbool.h>
#include "jogador.h"
#include "armadura.h"
#include "capacete.h"
#include "arma_principal.h"
#include "arma_secundaria.h"
#include "estado_habilidade.h"

void UI_SetEscala(float escala);
float UI_GetEscala(void);
float UI_AjustarTamanhoFonte(float base);
int UI_AjustarTamanhoFonteInt(float base);
bool UI_BotaoTexto(Rectangle rect, const char *texto, Vector2 mouse, bool clique,
                   Color corNormal, Color corHover, Font fonte, bool negrito);
void UI_DesenharQuadradoCooldown(Rectangle quad, const char *texto, float cooldownAtual,
                                 float cooldownMax, Font fonte);
void UI_DesenharPersonagemPreview(const Jogador *jogadorBase,
                                  Vector2 posicaoCentro,
                                  const Armadura *arm,
                                  const Capacete *cap,
                                  const ArmaPrincipal *armaPrincipal,
                                  const ArmaSecundaria *armaSecundaria,
                                  float escala);
void UI_DesenharEfeitoArmaSecundaria(const EstadoArmaSecundaria *estado,
                                           Vector2 posJogador);

#endif
