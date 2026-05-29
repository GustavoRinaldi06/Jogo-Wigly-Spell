#ifndef CORRGHOST_H
#define CORRGHOST_H

#include "Component.h"
#include "Rect_Vec2.h"
#include "Timer.h"
#include "SpriteRenderer.h"

class CorridorGhost : public Component
{
public:
    CorridorGhost(GameObject &associated,  const std::string &spritePath);
    ~CorridorGhost();
    void Start() override;
    void Update(float dt) override;
    void Render() override;
    
    bool Is(const std::string &type) override;
    void BounceATK(int offset);
    void MissileATK(int offset, int color);
    void NotifyCollision(GameObject &other) override;
    float health = 100;

    Timer BounceTimer;
    float bouncetime = 0.0;
    Timer MissSwap;
    float missTime = 5.0;
    int misscount = 1;
    int misstotal = 0;
    int swapcount = 3;
    int ATK = 0;
    bool attacked = false;
    bool colorsmnd = false;
    bool dead;
    Timer deathTimer;
    Timer specialInvuln;
    bool deathAnimTriggered{false};
};

#endif