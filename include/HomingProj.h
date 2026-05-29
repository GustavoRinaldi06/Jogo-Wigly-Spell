#ifndef HOMINGPROJ_H
#define HOMINGPROJ_H

#include "Component.h"
#include "Rect_Vec2.h"
#include "Timer.h"
#include "Sound.h"
#include "Bullet.h"
#include <queue>
#include <memory>

class HomingProj : public Component
{
public:

    HomingProj(GameObject &associated, const std::string &spritePath);
    ~HomingProj();

    void Start() override;
    void Update(float dt) override;
    void Render() override;
    bool Is(const std::string &type) override;

    // Colisão
    void NotifyCollision(GameObject &other) override;
    GameObject *GetGameObject() { return &associated; }

private:
    // Valores de controle plataforma
    Vec2 speed = Vec2(-20,0);
    float gravity = 200.0f; // Gravidade

    bool destroyed = false;
    int blockable = 5;

    // Death e hit
    Sound hitSound, fallSound, deathSound;
    Timer lifespan;
    Timer deathTimer;
    bool deathAnimTriggered{false};

    // Verificadores de inversão

    float linearSpeed = 50;
    
    
};

#endif
