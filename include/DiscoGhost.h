#ifndef DISCOGHOST_H
#define DISCOGHOST_H

#include "Component.h"
#include "Rect_Vec2.h"
#include "Timer.h"
#include "SpriteRenderer.h"

class DiscoGhost : public Component
{
public:
    DiscoGhost(GameObject &associated,  const std::string &spritePath);
    ~DiscoGhost();
    void Start() override;
    void Update(float dt) override;
    void Render() override;
    
    bool Is(const std::string &type) override;
    void NoteATK(int color);
    void WaveATK(int side);
    void SmnATK(int side, int pos);
    void NotifyCollision(GameObject &other) override;
    float health = 1500;

    Timer NoteTimer;
    float noteTime = 2.0;
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
    bool deathAnimTriggered{false};

    bool fightbegan = true;
};

#endif