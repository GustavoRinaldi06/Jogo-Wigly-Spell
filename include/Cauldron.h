#ifndef CAULDRON_H
#define CAULDRON_H

#include "Component.h"
#include "Rect_Vec2.h"
#include "Timer.h"
#include "Sound.h"
#include "Bullet.h"
#include "Animator.h"
#include <queue>
#include <memory>

class Cauldron : public Component
{
public:

    Cauldron(GameObject &associated, const std::string &spritePath, Animator *anim);
    ~Cauldron();

    void Start() override;
    void Update(float dt) override;
    void Render() override;
    bool Is(const std::string &type) override;
    // Colisão
    GameObject *GetGameObject() { return &associated; }
    Animator *Playeranim;
    float distance = 0;
    Timer shotTime;

};

#endif
