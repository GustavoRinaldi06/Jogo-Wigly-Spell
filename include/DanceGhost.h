#ifndef DANCEGHOST_H
#define DANCEGHOST_H

#include "Component.h"
#include "Rect_Vec2.h"
#include "Timer.h"
#include "SpriteRenderer.h"

class DanceGhost : public Component
{
public:
    DanceGhost(GameObject &associated,  const std::string &spritePath, int pos = -1);
    ~DanceGhost();
    void Start() override;
    void Update(float dt) override;
    void Render() override;
    
    bool Is(const std::string &type) override;
    void StarATK();
    void NotifyCollision(GameObject &other) override;
    float health = 100;

    Timer AtkTimer;
    float startime = 3.0;
    int misscount = 1;
    int misstotal = 0;
    int swapcount = 3;
    int ATK = 0;
    bool rushing = true;
    bool attacking = false;
    bool attacked = false;
    Vec2 speed = Vec2(0,0);
    bool dead;
    Timer deathTimer;
    Timer specialInvuln;
    bool deathAnimTriggered{false};
    int pos;
};

#endif