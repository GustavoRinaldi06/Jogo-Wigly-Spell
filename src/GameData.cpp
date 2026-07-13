#include "GameData.h"

int GameData::playerHP = 100;
int GameData::bossHP = 1000;
int GameData::gameMode = 0; // sempre inicializa no menu
int GameData::currentState = 0;
bool GameData::inverted = false;
Vec2 GameData::universalspeed = Vec2(0, 0);

bool GameData::playerVictory_1 = false; // Jogador ainda não ganhou

bool GameData::playerVictory_2 = false; // Jogador ainda não ganhou

bool GameData::playerVictory_3 = false; // Jogador ainda não ganhou

bool GameData::Djump = true;
bool GameData::spell = true;

bool GameData::finalfase = false;

bool GameData::aimed = true;
bool GameData::isMuted = false;
bool GameData::discostart = false;

bool GameData::summonalive[6] = {false, false, false, false, false, false};
bool GameData::discopos[14] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false};
int GameData::inversedisco = 0;
bool GameData::tripledisco = false;
int GameData::target_stage = 1;

// No seu GameData.cpp
int GameData::targetDiscoColor = 0;
bool GameData::discoAttackActive = false;
bool GameData::discoBlackout = false;
bool GameData::discoError = false;
bool GameData::shotFired = false;

bool GameData::expert = false;
bool GameData::easy = false;
float  GameData::p2health = -1000;
float  GameData::p3health = -1000;

DanceFloor *GameData::danceFloorPtr = nullptr; // pista fase 3