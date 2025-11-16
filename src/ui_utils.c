#include "ui_utils.h"
#include <math.h>
#include <string.h>

static float gEscalaUI = 1.0f;

void UI_SetEscala(float escala)
{
    gEscalaUI = escala;
}

float UI_GetEscala(void)
{
    return gEscalaUI;
}

float UI_AjustarTamanhoFonte(float base)
{
    float valor = base * gEscalaUI;
    float minimo = base * 0.6f;
    if (valor < minimo) valor = minimo;
    if (valor < 14.0f) valor = 14.0f;
    return valor;
}

int UI_AjustarTamanhoFonteInt(float base)
{
    return (int)lroundf(UI_AjustarTamanhoFonte(base));
}

bool UI_BotaoTexto(Rectangle rect, const char *texto, Vector2 mouse, bool clique,
                   Color corNormal, Color corHover, Font fonte, bool negrito)
{
    bool hover = CheckCollisionPointRec(mouse, rect);
    DrawRectangleRounded(rect, 0.2f, 8, hover ? corHover : corNormal);
    float tamanho = UI_AjustarTamanhoFonte(negrito ? 24.0f : 20.0f);
    Vector2 medida = MeasureTextEx(fonte, texto, tamanho, 1.0f);
    DrawTextEx(fonte,
               texto,
               (Vector2){ rect.x + rect.width / 2 - medida.x / 2,
                          rect.y + rect.height / 2 - medida.y / 2 },
               tamanho,
               1.0f,
               hover ? BLACK : WHITE);
    return hover && clique;
}

void UI_DesenharQuadradoCooldown(Rectangle quad, const char *texto, float cooldownAtual,
                                 float cooldownMax, Font fonte)
{
    DrawRectangleRounded(quad, 0.15f, 6, (Color){30, 30, 50, 230});
    DrawRectangleRoundedLines(quad, 0.15f, 6, (Color){70, 70, 120, 255});
    float textoTam = UI_AjustarTamanhoFonte(18.0f);
    Vector2 medida = MeasureTextEx(fonte, texto, textoTam, 1.0f);
    DrawTextEx(fonte, texto,
               (Vector2){ quad.x + quad.width / 2 - medida.x / 2, quad.y + 6.0f * gEscalaUI },
               textoTam, 1.0f, SKYBLUE);
    float preenchimento = 0.0f;
    if (cooldownMax > 0.0f) {
        preenchimento = 1.0f - (cooldownAtual / cooldownMax);
        if (preenchimento < 0.0f) preenchimento = 0.0f;
        if (preenchimento > 1.0f) preenchimento = 1.0f;
    }
    float padding = 8.0f * gEscalaUI;
    float barraAltura = fmaxf(8.0f, 10.0f * gEscalaUI);
    Rectangle barra = { quad.x + padding, quad.y + quad.height - (barraAltura + padding),
                        (quad.width - padding * 2.0f) * preenchimento, barraAltura };
    DrawRectangleRounded(barra, 0.2f, 4, (Color){120, 190, 255, 255});
}

void UI_DesenharPersonagemPreview(const Jogador *jogadorBase,
                                  Vector2 posicaoCentro,
                                  const Armadura *arm,
                                  const Capacete *cap,
                                  const ArmaPrincipal *armaPrincipal,
                                  const ArmaSecundaria *armaSecundaria,
                                  float escala)
{
    if (!jogadorBase) return;
    if (escala <= 0.0f) escala = 1.0f;
    const Texture2D sprite = jogadorBase->parado;
    DrawTextureEx(sprite,
                  (Vector2){ posicaoCentro.x - (sprite.width * escala) / 2.0f,
                             posicaoCentro.y - (sprite.height * escala) / 2.0f },
                  0.0f,
                  escala,
                  WHITE);

    if (cap) DesenharCapacete(cap, posicaoCentro, escala);
    if (arm) DesenharArmadura(arm, posicaoCentro, false, false, escala);
    if (armaSecundaria) DesenharArmaSecundaria(armaSecundaria, posicaoCentro, false, false, escala);
    if (armaPrincipal) DesenharArmaPrincipal(armaPrincipal, posicaoCentro, false, false, escala);
}

void UI_DesenharEfeitoArmaSecundaria(const EstadoArmaSecundaria *estado,
                                           Vector2 posJogador)
{
    if (!estado || !estado->ativo || !estado->dados) return;
    Vector2 centro = estado->segueJogador ? posJogador : estado->centro;
    Color corPrincipal = (Color){180, 200, 255, 120};
    Color corLinha = (Color){220, 240, 255, 200};

    switch (estado->dados->tipo) {
        case TIPO_ARMA_SECUNDARIA_AREA_PULSANTE: {
            float raio = estado->dados->raioOuAlcance > 0 ? estado->dados->raioOuAlcance : 120.0f;
            DrawCircleV(centro, raio, ColorAlpha(ORANGE, 0.35f));
            DrawCircleLines((int)centro.x, (int)centro.y, raio, ColorAlpha(RED, 0.8f));
        } break;
        case TIPO_ARMA_SECUNDARIA_CONE_EMPURRAO: {
            float alcance = estado->dados->raioOuAlcance > 0 ? estado->dados->raioOuAlcance : 180.0f;
            float abertura = 70.0f;
            float angulo = atan2f(estado->direcao.y, estado->direcao.x) * (180.0f / PI);
            DrawCircleSector(centro, alcance, angulo - abertura * 0.5f, angulo + abertura * 0.5f, 0, ColorAlpha(corPrincipal, 0.5f));
            DrawCircleSectorLines(centro, alcance, angulo - abertura * 0.5f, angulo + abertura * 0.5f, 0, corLinha);
        } break;
        case TIPO_ARMA_SECUNDARIA_DEFESA_TEMPORARIA: {
            float raio = estado->dados->raioOuAlcance > 0 ? estado->dados->raioOuAlcance : 90.0f;
            DrawCircleV(centro, raio, ColorAlpha(SKYBLUE, 0.4f));
            DrawCircleLines((int)centro.x, (int)centro.y, raio, ColorAlpha(BLUE, 0.8f));
        } break;
        case TIPO_ARMA_SECUNDARIA_ZONA_LENTIDAO: {
            float raio = estado->dados->raioOuAlcance > 0 ? estado->dados->raioOuAlcance : 160.0f;
            DrawCircleV(centro, raio, ColorAlpha(DARKBLUE, 0.3f));
            DrawCircleLines((int)centro.x, (int)centro.y, raio, ColorAlpha(BLUE, 0.7f));
        } break;
        default:
            break;
    }
}
