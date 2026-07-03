#ifndef BUBBLECD_H
#define BUBBLECD_H

#include "Component.h"
#include "Rect_Vec2.h"
#include "Timer.h"
#include "Sound.h"
#include "Bullet.h"
#include <queue>
#include <memory>

class BubbleCD : public Component
{
public:

    BubbleCD(GameObject &associated, const std::string &spritePath);
    ~BubbleCD();

    void Start() override;
    void Update(float dt) override;
    void Render() override;
    bool Is(const std::string &type) override;
    // Colisão
    GameObject *GetGameObject() { return &associated; }

};

#endif
