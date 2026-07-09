#ifndef SMOKE_H
#define SMOKE_H

#include "Component.h"
#include "Rect_Vec2.h"
#include "Timer.h"
#include "Sound.h"
#include "Bullet.h"
#include <queue>
#include <memory>

class Smoke : public Component
{
public:

    Smoke(GameObject &associated, const std::string &spritePath, Vec2 pos, Vec2 scale, int type = 0);
    ~Smoke();

    void Start() override;
    void Update(float dt) override;
    void Render() override;
    bool Is(const std::string &type) override;
    static void CreateSmoke(Vec2 pos, Vec2 scale, int type = 0);
    // Colisão
    GameObject *GetGameObject() { return &associated; }

};

#endif
