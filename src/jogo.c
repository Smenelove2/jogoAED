#include "jogo.h"
#include "ui_utils.h"
#include "mapa.h"
#include "jogo_config.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define RAYGUN_PROJETIL_VELOCIDADE 650.0f

static float ComprimentoV2(Vector2 v)
{
    return sqrtf(v.x * v.x + v.y * v.y);
}

static Vector2 NormalizarV2(Vector2 v)
{
    float len = ComprimentoV2(v);
    if (len <= 0.0001f) return (Vector2){1.0f, 0.0f};
    return (Vector2){ v.x / len, v.y / len };
}

void JogoInicializar(EstadoJogo *estado, float regeneracaoBase)
{
    if (!estado) return;
    memset(estado, 0, sizeof(*estado));
    estado->regeneracaoAtual = regeneracaoBase;
    estado->solicitouRetornoMenu = false;
}

void JogoReiniciar(EstadoJogo *estado, Jogador *jogador, Camera2D *camera, Vector2 posInicial)
{
    if (!estado || !jogador || !camera) return;
    estado->raygun = (RaygunProjetil){0};
    estado->armaSecundaria = (EstadoArmaSecundaria){0};
    estado->cooldownArmaSecundaria = 0.0f;
    estado->tempoMensagemAtaque = 0.0f;
    estado->mensagemAtaque[0] = '\0';
    estado->pausado = false;
    estado->efeitoArmaPrincipal.ativo = false;
    estado->solicitouRetornoMenu = false;
    jogador->posicao = posInicial;
    camera->target = jogador->posicao;
}

static void AtualizarRaygun(EstadoJogo *estado, float dt)
{
    if (!estado || !estado->raygun.ativo) return;
    estado->raygun.posicao.x += estado->raygun.velocidade.x * dt;
    estado->raygun.posicao.y += estado->raygun.velocidade.y * dt;
    estado->raygun.tempoRestante -= dt;
    if (estado->raygun.tempoRestante <= 0.0f) {
        estado->raygun.ativo = false;
        estado->raygun.arma = NULL;
        snprintf(estado->mensagemAtaque, sizeof(estado->mensagemAtaque), "RayGun impactou!");
        estado->tempoMensagemAtaque = 0.5f;
    }
}

void JogoAtualizar(EstadoJogo *estado,
                   Jogador *jogador,
                   Camera2D *camera,
                   Mapa **mapa,
                   int linhasMapa,
                   int colunasMapa,
                   int tileLargura,
                   int tileAltura,
                   float dt,
                   Vector2 mouseNoMundo,
                   bool mouseClickEsq,
                   bool mouseClickDir,
                   bool escapePress,
                   const Armadura *armaduraAtual,
                   const Capacete *capaceteAtual,
                   ArmaPrincipal *armaPrincipalAtual,
                   const ArmaSecundaria *armaSecundariaAtual)
{
    if (!estado || !jogador || !camera) return;
    (void)armaduraAtual;
    (void)capaceteAtual;

    if (escapePress) estado->pausado = !estado->pausado;

    if (estado->cooldownArmaSecundaria > 0.0f) {
        estado->cooldownArmaSecundaria -= dt;
        if (estado->cooldownArmaSecundaria < 0.0f) estado->cooldownArmaSecundaria = 0.0f;
    }

    if (estado->armaSecundaria.ativo) {
        estado->armaSecundaria.tempoRestante -= dt;
        if (estado->armaSecundaria.segueJogador) {
            estado->armaSecundaria.centro = jogador->posicao;
        }
        if (estado->armaSecundaria.tempoRestante <= 0.0f) {
            estado->armaSecundaria.ativo = false;
            estado->armaSecundaria.dados = NULL;
        }
    }

    if (!estado->pausado) {
        AtualizarArmaPrincipal(armaPrincipalAtual, dt);
        AtualizarEfeitoArmaPrincipal(&estado->efeitoArmaPrincipal, dt);
        if (estado->tempoMensagemAtaque > 0.0f) {
            estado->tempoMensagemAtaque -= dt;
            if (estado->tempoMensagemAtaque < 0.0f) estado->tempoMensagemAtaque = 0.0f;
        }

        Vector2 posAnterior = jogador->posicao;
        AtualizarJogador(jogador, dt);
        AtualizarNoAtualJogador(jogador, mapa, linhasMapa, colunasMapa, tileLargura, tileAltura);

        if (jogador->noAtual && jogador->noAtual->colisao) {
            jogador->posicao = posAnterior;
            AtualizarNoAtualJogador(jogador, mapa, linhasMapa, colunasMapa, tileLargura, tileAltura);
        }

        float maxX = colunasMapa * (float)tileLargura - 1.0f;
        float maxY = linhasMapa * (float)tileAltura - 1.0f;
        if (jogador->posicao.x < 0) jogador->posicao.x = 0;
        if (jogador->posicao.y < 0) jogador->posicao.y = 0;
        if (jogador->posicao.x > maxX) jogador->posicao.x = maxX;
        if (jogador->posicao.y > maxY) jogador->posicao.y = maxY;

        camera->target = jogador->posicao;

        if (armaPrincipalAtual && mouseClickEsq) {
            bool ehRaygun = armaPrincipalAtual && (strcmp(armaPrincipalAtual->nome, "RayGun") == 0);
            if (ehRaygun) {
                if (PodeAtacarArmaPrincipal(armaPrincipalAtual)) {
                    Vector2 delta = { mouseNoMundo.x - jogador->posicao.x, mouseNoMundo.y - jogador->posicao.y };
                    Vector2 direcao = NormalizarV2(delta);
                    float distancia = ComprimentoV2(delta);
                    if (distancia <= 0.01f) distancia = armaPrincipalAtual->alcanceMaximo;
                    float alcanceUtil = fminf(distancia, armaPrincipalAtual->alcanceMaximo);
                    if (alcanceUtil <= 0.0f) alcanceUtil = armaPrincipalAtual->alcanceMaximo;
                    Vector2 destino = {
                        jogador->posicao.x + direcao.x * alcanceUtil,
                        jogador->posicao.y + direcao.y * alcanceUtil
                    };

                    armaPrincipalAtual->tempoRecargaRestante = armaPrincipalAtual->tempoRecarga;
                    estado->raygun.ativo = true;
                    estado->raygun.arma = armaPrincipalAtual;
                    estado->raygun.origem = jogador->posicao;
                    estado->raygun.posicao = jogador->posicao;
                    estado->raygun.destino = destino;
                    estado->raygun.direcao = direcao;
                    estado->raygun.velocidade = (Vector2){
                        direcao.x * RAYGUN_PROJETIL_VELOCIDADE,
                        direcao.y * RAYGUN_PROJETIL_VELOCIDADE
                    };
                    float distProj = ComprimentoV2((Vector2){ destino.x - jogador->posicao.x, destino.y - jogador->posicao.y });
                    float tempoProj = distProj / RAYGUN_PROJETIL_VELOCIDADE;
                    if (tempoProj < 0.08f) tempoProj = 0.08f;
                    estado->raygun.tempoRestante = tempoProj;
                    snprintf(estado->mensagemAtaque, sizeof(estado->mensagemAtaque), "Disparo RayGun!");
                    estado->tempoMensagemAtaque = 0.5f;
                } else {
                    snprintf(estado->mensagemAtaque, sizeof(estado->mensagemAtaque), "Recarga: %.1fs", armaPrincipalAtual->tempoRecargaRestante);
                    estado->tempoMensagemAtaque = 1.0f;
                }
            } else {
                if (PodeAtacarArmaPrincipal(armaPrincipalAtual)) {
                    bool sucesso = AcionarAtaqueArmaPrincipal(
                        armaPrincipalAtual,
                        jogador->posicao,
                        mouseNoMundo,
                        &estado->efeitoArmaPrincipal
                    );
                    if (sucesso) {
                        snprintf(estado->mensagemAtaque, sizeof(estado->mensagemAtaque), "Ataque: %s", armaPrincipalAtual->nome);
                    } else {
                        snprintf(estado->mensagemAtaque, sizeof(estado->mensagemAtaque), "Fora do alcance!");
                    }
                } else {
                    snprintf(estado->mensagemAtaque, sizeof(estado->mensagemAtaque), "Recarga: %.1fs", armaPrincipalAtual->tempoRecargaRestante);
                }
                estado->tempoMensagemAtaque = 1.0f;
            }
        }

        if (estado->raygun.ativo) {
            AtualizarRaygun(estado, dt);
        }

        if (estado->regeneracaoAtual > 0.0f && jogador->vida < jogador->vidaMaxima) {
            jogador->vida += estado->regeneracaoAtual * dt;
            if (jogador->vida > jogador->vidaMaxima) jogador->vida = jogador->vidaMaxima;
        }

        if (armaSecundariaAtual && mouseClickDir) {
            if (estado->armaSecundaria.ativo || estado->cooldownArmaSecundaria > 0.0f) {
                snprintf(estado->mensagemAtaque, sizeof(estado->mensagemAtaque), "Arma Secundaria CD: %.1fs", estado->cooldownArmaSecundaria);
                estado->tempoMensagemAtaque = 1.0f;
            } else {
                estado->cooldownArmaSecundaria = armaSecundariaAtual->tempoRecarga;
                estado->armaSecundaria.ativo = true;
                estado->armaSecundaria.dados = armaSecundariaAtual;
                estado->armaSecundaria.tempoRestante = armaSecundariaAtual->duracao;
                estado->armaSecundaria.centro = jogador->posicao;
                Vector2 dir = NormalizarV2((Vector2){ mouseNoMundo.x - jogador->posicao.x,
                                                      mouseNoMundo.y - jogador->posicao.y });
                estado->armaSecundaria.direcao = dir;
                estado->armaSecundaria.segueJogador =
                    (armaSecundariaAtual->tipo == TIPO_ARMA_SECUNDARIA_DEFESA_TEMPORARIA) ||
                    (armaSecundariaAtual->tipo == TIPO_ARMA_SECUNDARIA_AREA_PULSANTE);
                if (armaSecundariaAtual->tipo == TIPO_ARMA_SECUNDARIA_ZONA_LENTIDAO) {
                    estado->armaSecundaria.centro = mouseNoMundo;
                } else if (armaSecundariaAtual->tipo == TIPO_ARMA_SECUNDARIA_CONE_EMPURRAO) {
                    estado->armaSecundaria.centro = jogador->posicao;
                }
                snprintf(estado->mensagemAtaque, sizeof(estado->mensagemAtaque), "Arma Secundaria: %s", armaSecundariaAtual->nome);
                estado->tempoMensagemAtaque = 1.0f;
            }
        }
    } else {
        AtualizarRaygun(estado, dt);
    }
}

void JogoDesenhar(EstadoJogo *estado,
                  const Jogador *jogador,
                  const Camera2D *camera,
                  Mapa **mapa,
                  Texture2D *tiles,
                  int quantidadeTiles,
                  int linhasMapa,
                  int colunasMapa,
                  int tileLargura,
                  int tileAltura,
                  int largura, int altura,
                  Font fonteNormal,
                  Font fonteBold,
                  const Armadura *armaduraAtual,
                  const Capacete *capaceteAtual,
                  const ArmaPrincipal *armaPrincipalAtual,
                  const ArmaSecundaria *armaSecundariaAtual,
                  Vector2 mousePos,
                  bool mouseClick)
{
    if (!estado || !jogador || !camera) return;
    (void)fonteNormal;
    const float escalaUI = UI_GetEscala();

    BeginMode2D(*camera);
        DesenharMapaVisivel((Camera2D *)camera, largura, altura,
                            mapa, linhasMapa, colunasMapa,
                            tiles, quantidadeTiles,
                            tileLargura, tileAltura);

        DesenharEfeitoArmaPrincipal(&estado->efeitoArmaPrincipal);
        UI_DesenharEfeitoArmaSecundaria(&estado->armaSecundaria, jogador->posicao);
        if (estado->raygun.ativo) {
            DrawCircleV(estado->raygun.posicao, 6.0f, SKYBLUE);
        }
        const float escalaEquipamento = ESCALA_MUNDO;
        DesenharJogador(jogador);
        if (capaceteAtual) {
            DesenharCapacete(capaceteAtual, jogador->posicao, escalaEquipamento);
        }
        if (armaduraAtual) {
            DesenharArmadura(armaduraAtual, jogador->posicao, jogador->emMovimento, jogador->alternarFrame, escalaEquipamento);
        }
        if (armaSecundariaAtual) {
            DesenharArmaSecundaria(armaSecundariaAtual, jogador->posicao, jogador->emMovimento, jogador->alternarFrame, escalaEquipamento);
        }
        if (armaPrincipalAtual) {
            DesenharArmaPrincipal(armaPrincipalAtual, jogador->posicao, jogador->emMovimento, jogador->alternarFrame, escalaEquipamento);
        }
    EndMode2D();

    DrawText("ESC para pausar", 20, 20, UI_AjustarTamanhoFonteInt(20.0f), WHITE);

    float vidaAtual = jogador->vida;
    float vidaMax = jogador->vidaMaxima;
    if (vidaMax <= 0.0f) vidaMax = 1.0f;
    int margemBase = (int)lroundf(20.0f * escalaUI);
    float barraLarg = 300.0f * escalaUI;
    float barraAlt = 28.0f * escalaUI;
    Rectangle barraBase = { (float)margemBase, altura - barraAlt - margemBase, barraLarg, barraAlt };
    DrawRectangleRounded(barraBase, 0.2f, 8, (Color){40, 40, 60, 255});
    float preenchimento = (vidaAtual / vidaMax);
    if (preenchimento < 0.0f) preenchimento = 0.0f;
    if (preenchimento > 1.0f) preenchimento = 1.0f;
    float paddingBarra = 4.0f * escalaUI;
    Rectangle barraVida = { barraBase.x + paddingBarra, barraBase.y + paddingBarra,
                            (barraBase.width - paddingBarra * 2.0f) * preenchimento, barraBase.height - paddingBarra * 2.0f };
    DrawRectangleRounded(barraVida, 0.2f, 8, (Color){200, 60, 60, 255});
    float textoVidaTam = UI_AjustarTamanhoFonte(20.0f);
    DrawTextEx(fonteBold,
               TextFormat("Vida: %.0f / %.0f", vidaAtual, vidaMax),
               (Vector2){ barraBase.x + 10.0f * escalaUI, barraBase.y - textoVidaTam - 4.0f },
               textoVidaTam, 1.0f, WHITE);

    float quadSize = 90.0f * escalaUI;
    float padding = 12.0f * escalaUI;
    Rectangle quadDir = { largura - quadSize - padding, altura - quadSize - padding, quadSize, quadSize };
    Rectangle quadEsq = { quadDir.x - quadSize - padding, quadDir.y, quadSize, quadSize };

    float cdPrincipal = armaPrincipalAtual ? armaPrincipalAtual->tempoRecargaRestante : 0.0f;
    float cdPrincipalMax = armaPrincipalAtual ? armaPrincipalAtual->tempoRecarga : 1.0f;
    UI_DesenharQuadradoCooldown(quadEsq, "Mouse ESQ", cdPrincipal, cdPrincipalMax, fonteBold);

    float cdSec = estado->cooldownArmaSecundaria;
    float cdSecMax = (armaSecundariaAtual) ? armaSecundariaAtual->tempoRecarga : 1.0f;
    UI_DesenharQuadradoCooldown(quadDir, "Mouse DIR", cdSec, cdSecMax, fonteBold);

    if (estado->tempoMensagemAtaque > 0.0f && estado->mensagemAtaque[0] != '\0') {
        int textoMsg = UI_AjustarTamanhoFonteInt(18.0f);
        float espacoMsg = 8.0f * escalaUI;
        float msgY = barraBase.y - textoVidaTam - espacoMsg - textoMsg;
        if (msgY < margemBase) msgY = margemBase;
        DrawText(estado->mensagemAtaque, (int)barraBase.x, (int)msgY, textoMsg, YELLOW);
    }

    if (estado->pausado) {
        DrawRectangle(0, 0, largura, altura, ColorAlpha(BLACK, 0.5f));
        const char *tituloPausa = "Jogo Pausado";
        float pausaTam = UI_AjustarTamanhoFonte(40.0f);
        Vector2 tituloMedida = MeasureTextEx(fonteBold, tituloPausa, pausaTam, 1.0f);
        DrawTextEx(fonteBold, tituloPausa, (Vector2){ largura / 2.0f - tituloMedida.x / 2.0f, altura / 3.0f }, pausaTam, 1.0f, WHITE);

        float larguraBtn = 260.0f * escalaUI;
        float alturaBtn = 60.0f * escalaUI;
        float espacoBtn = 20.0f * escalaUI;
        Rectangle btnRetomar = { largura / 2.0f - larguraBtn / 2.0f, altura / 2.0f - alturaBtn - espacoBtn * 0.5f, larguraBtn, alturaBtn };
        Rectangle btnMenu = { largura / 2.0f - larguraBtn / 2.0f, altura / 2.0f + espacoBtn * 0.5f, larguraBtn, alturaBtn };

        if (UI_BotaoTexto(btnRetomar, "Retomar", mousePos, mouseClick,
                           (Color){80, 120, 80, 255}, (Color){180, 220, 180, 255}, fonteBold, true)) {
            estado->pausado = false;
        }
        if (UI_BotaoTexto(btnMenu, "Sair para o Menu", mousePos, mouseClick,
                           (Color){120, 60, 60, 255}, (Color){220, 120, 120, 255}, fonteBold, true)) {
            estado->pausado = false;
            estado->raygun.ativo = false;
            estado->armaSecundaria.ativo = false;
            estado->armaSecundaria.dados = NULL;
            estado->cooldownArmaSecundaria = 0.0f;
            estado->mensagemAtaque[0] = '\0';
            estado->tempoMensagemAtaque = 0.0f;
            estado->solicitouRetornoMenu = true;
        }
    }
}
