#ifndef GAMEDATA_H
#define GAMEDATA_H
#include "Rect_Vec2.h"

class GameData
{
public:
    static int playerHP;
    static int bossHP;

    // Modo de jogo/mapa
    static int gameMode; // 0 se for o menu, 1 se for uma fase (plataforma)
    static int currentState; // 0 = fora de fase, 1/2/3 para respectivas 3 fases
    static bool inverted; //Gravidade e pulo invertido

    // Historico de jogo e condição de vitoria
    static bool playerVictory_1; // SAber se o jogador ganhou ou perdeu a fase 1
    static bool complete1; // SAber se completou a fase 1 já (usar para save)

    static bool playerVictory_2; // SAber se o jogador ganhou ou perdeu a fase 2
    static bool complete2;       // SAber se completou a fase 2 já (usar para save)

    static bool playerVictory_3; // SAber se o jogador ganhou ou perdeu a fase 3
    static bool complete3;       // SAber se completou a fase 3 já (usar para save)

    static Vec2 universalspeed;

    // Habilidades desbloqueaveis
    static bool Djump;
    static bool spell;

    // Colocar com mira ou sem mira
    static bool aimed;

    static bool isMuted;
    static bool summonalive[6];
};

#endif // GAMEDATA_H
