#ifndef FLOOR_H
#define FLOOR_H

#include "Component.h"
#include "Rect_Vec2.h"
#include "Timer.h"
#include "Sound.h"
#include "Bullet.h"
#include <queue>
#include <memory>

class Floor : public Component
{
public:

    Floor(GameObject &associated, const std::string &spritePath, int frame, SDL_RendererFlip flip, float angle, int type = 0,int index = -1);
    ~Floor();

    void Start() override;
    void Update(float dt) override;
    void Render() override;
    bool Is(const std::string &type) override;
    Vec2 speed = Vec2(0,0);
    int limit = -120;
    int type;
    int index = -1;
    Timer changeTime;
    // Colisão
    GameObject *GetGameObject() { return &associated; }

};

#endif
