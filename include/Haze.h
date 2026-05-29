#ifndef HAZE_H
#define HAZE_H

#include "Component.h"
#include "Rect_Vec2.h"
#include "Timer.h"
#include "Sound.h"
#include <queue>
#include <memory>

class Haze : public Component
{
public:

    Haze(GameObject &associated, const std::string &spritePath, SDL_RendererFlip flip, float angle);
    ~Haze();

    void Start() override;
    void Update(float dt) override;
    void Render() override;
    bool Is(const std::string &type) override;
    Vec2 speed = Vec2(0,0);
    int limit = -120;
    // Colisão
    GameObject *GetGameObject() { return &associated; }

};

#endif
