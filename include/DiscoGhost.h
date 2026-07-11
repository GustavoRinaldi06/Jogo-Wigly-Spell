#ifndef DISCOGHOST_H
#define DISCOGHOST_H

#include "Component.h"
#include "Rect_Vec2.h"
#include "Timer.h"
#include "Sound.h"
#include "SpriteRenderer.h"

class Text;

class DiscoGhost : public Component
{
public:
    DiscoGhost(GameObject &associated,  const std::string &spritePath);
    ~DiscoGhost();
    void Start() override;
    void Update(float dt) override;
    void Render() override;
    
    bool Is(const std::string &type) override;
    void NoteATK(int color, int fixed_po = 0);
    void WaveATK(int side, Vec2 offset = Vec2(0,0));
    void SmnATK(int side, int pos);
    void NotifyCollision(GameObject &other) override;
    float health = 1500;

    Timer NoteTimer;
    float noteTime = 2.0;
    int last_color = -1;
    Timer AtkTimer;
    float atkTime = 7.0;
    Timer SmnTimer;
    float smnTime = 10.0;
    Timer DiscoTimer;
    float discoTime = 20.0;
    Timer AnimTimer;
    int lastnote = -1;
    int swapcount = 2;
    int ATK = 0;
    bool attacked = false;
    bool colorsmnd = false;
    bool dead;
    Timer deathTimer;
    Timer specialInvuln;
    Timer waveCD;
    bool deathAnimTriggered{false};
    int nextwave = 0;
    int finalfase = 0;
    int finalnote = 0;

    bool fightbegan = true;

private:
    // Variáveis para o ataque 4
    Timer discoRoundTimer;         // cronômetro para contar tempo de cada round
    int discoCurrentRound = 0;     // round atual
    int targetColor = -1;          // armazena a cor correta da rodada
    bool roundActive = false;      // se o round atual está em andamento
    Text *discoInfoText = nullptr; // TEXTO boss
    bool feedbackActive = false;   // Tempo de feedback
    bool acertou = false;          // Acertou no tutorial
    int animcounter = 0;
    Sound Error, Right_Place;
};

#endif