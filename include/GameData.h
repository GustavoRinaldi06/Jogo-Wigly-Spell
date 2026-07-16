#ifndef GAMEDATA_H
#define GAMEDATA_H
#include "Rect_Vec2.h"

class DanceFloor;

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

    static bool playerVictory_2; // SAber se o jogador ganhou ou perdeu a fase 2

    static bool playerVictory_3; // SAber se o jogador ganhou ou perdeu a fase 3

    // Historico de jogo e condição de vitoria
    static bool Permanent_playerVictory_1; // SAber se o jogador venceu a fase 1 alguma vez

    static bool Permanent_playerVictory_2; // SAber se o jogador venceu a fase 2 alguma vez

    static bool Permanent_playerVictory_3; // SAber se o jogador venceu a fase 3 alguma vez

    static bool cheats;

    static Vec2 universalspeed;

    // Habilidades desbloqueaveis
    static bool Djump;
    static bool spell;

    // Colocar com mira ou sem mira
    static bool aimed;

    static bool isMuted;
    static bool summonalive[6];
    static bool finalfase;
    static bool discostart;
    static bool discopos[14];
    static int inversedisco;
    static bool tripledisco;
    static int target_stage;

    // controle de ataque do disco
    static int targetDiscoColor;
    static bool discoAttackActive;
    static bool discoBlackout;
    static bool discoError;
    static bool shotFired;
    static bool expert;
    static bool easy;
    static float p2health;
    static float p3health;

    static DanceFloor *danceFloorPtr;
};

#endif // GAMEDATA_H
