#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Component.h"
#include "Rect_Vec2.h"
#include "Timer.h"
#include "Sound.h"
#include "Bullet.h"
#include <queue>
#include <memory>

class Background : public Component
{
public:

    Background(GameObject &associated, const std::string &spritePath, int frame, SDL_RendererFlip flip, float layer, float followrate);
    ~Background();

    void Start() override;
    void Update(float dt) override;
    void Render() override;
    bool Is(const std::string &type) override;
    Vec2 speed = Vec2(0,0);
    int limit = -1200;
    float frate = 1.0;
    // Colisão
    GameObject *GetGameObject() { return &associated; }

};

#endif
