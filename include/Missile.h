#ifndef MISSILE_H
#define MISSILE_H

#include "Component.h"
#include "Rect_Vec2.h"
#include "Timer.h"
#include "Sound.h"
#include "Bullet.h"
#include <queue>
#include <memory>

class Missile : public Component
{
public:

    Missile(GameObject &associated, const std::string &spritePath, int color = 0);
    ~Missile();

    void Start() override;
    void Update(float dt) override;
    void Render() override;
    bool Is(const std::string &type) override;

    // Colisão
    void NotifyCollision(GameObject &other) override;
    GameObject *GetGameObject() { return &associated; }

private:
    // Valores de controle plataforma
    Vec2 speed = Vec2(0,0);

    bool destroyed = false;
    int blockable = 9;

    // Death e hit
    Sound hitSound, fallSound, deathSound;
    Timer lifespan;
    Timer deathTimer;
    bool deathAnimTriggered{false};
    

    // Verificadores de inversão
    Timer LockinTimer;
    bool launched = false;
    float linearSpeed = 600;
    
    
};

#endif
