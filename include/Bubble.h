#ifndef BUBBLE_H
#define BUBBLE_H

#include "Component.h"
#include "Rect_Vec2.h"
#include "Timer.h"
#include "Sound.h"
#include "Bullet.h"
#include <queue>
#include <memory>

class Bubble : public Component
{
public:

    Bubble(GameObject &associated, const std::string &spritePath);
    ~Bubble();

    void Start() override;
    void Update(float dt) override;
    void Render() override;
    bool Is(const std::string &type) override;
    // Colisão
    GameObject *GetGameObject() { return &associated; }

};

#endif
