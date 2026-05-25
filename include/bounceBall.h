#ifndef BOUNCEBALL_H
#define BOUNCEBALL_H

#include "Component.h"
#include "Rect_Vec2.h"
#include "Timer.h"
#include "Sound.h"
#include "Bullet.h"
#include <queue>
#include <memory>

class BounceBall : public Component
{
public:

    BounceBall(GameObject &associated, const std::string &spritePath);
    ~BounceBall();

    void Start() override;
    void Update(float dt) override;
    void Render() override;
    bool Is(const std::string &type) override;

    // Colisão
    void NotifyCollision(GameObject &other) override;
    GameObject *GetGameObject() { return &associated; }

private:
    // Valores de controle plataforma
    Vec2 speed = Vec2(-300,0);
    float gravity = 1000.0f; // Gravidade
    float basespeedy = 500;

    bool destroyed = false;
    bool blockable = true;

    // Death e hit
    Sound hitSound, fallSound, deathSound;
    Timer bounceTimer;
    Timer lifespan;
    Timer deathTimer;
    bool deathAnimTriggered{false};

    // Verificadores de inversão
    bool bouncing;

    float linearSpeed;
    
    
};

#endif
