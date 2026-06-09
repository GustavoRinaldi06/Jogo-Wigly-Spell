#ifndef BEATWAVE_H
#define BEATWAVE_H

#include "Component.h"
#include "Rect_Vec2.h"
#include "Timer.h"
#include "Sound.h"
#include "Bullet.h"
#include <queue>
#include <memory>

class BeatWave : public Component
{
public:

    BeatWave(GameObject &associated, const std::string &spritePath);
    ~BeatWave();

    void Start() override;
    void Update(float dt) override;
    void Render() override;
    bool Is(const std::string &type) override;

    // Colisão
    void NotifyCollision(GameObject &other) override;
    GameObject *GetGameObject() { return &associated; }

private:
    // Valores de controle plataforma
    Vec2 speed = Vec2(-50,0);
    

    bool destroyed = false;


    // Death e hit
    Sound hitSound, fallSound, deathSound;
    Timer lifespan;
    Timer deathTimer;
    bool active = false;
    bool deathAnimTriggered{false};

    // Verificadores de inversão
    bool bouncing = false;
    int blockable = 5;

    float linearSpeed;
    
    
};

#endif
