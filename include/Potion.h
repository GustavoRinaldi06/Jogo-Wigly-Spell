#ifndef POTION_H
#define POTION_H

#include "Component.h"
#include "Rect_Vec2.h"
#include "Timer.h"
#include "Sound.h"
#include "Bullet.h"
#include <queue>
#include <memory>

class Potion : public Component
{
public:

    Potion(GameObject &associated, const std::string &spritePath, int num);
    ~Potion();

    void Start() override;
    void Update(float dt) override;
    void Render() override;
    bool Is(const std::string &type) override;
    // Colisão
    GameObject *GetGameObject() { return &associated; }
    int number;

};

#endif
