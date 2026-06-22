#include "GameData.h"

int GameData::playerHP = 100;
int GameData::bossHP = 1000;
int GameData::gameMode = 0; // sempre inicializa no menu
int GameData::currentState = 0;
bool GameData::inverted = false;
Vec2 GameData::universalspeed = Vec2(0,0);

bool GameData::playerVictory_1 = false; // Jogador ainda não ganhou

bool GameData::playerVictory_2 = false; // Jogador ainda não ganhou

bool GameData::playerVictory_3 = false; // Jogador ainda não ganhou

bool GameData::Djump = true;
bool GameData::spell = true;

bool GameData::aimed = true;
bool GameData::isMuted = false;

bool GameData::summonalive[6] = {false,false,false,false,false,false};