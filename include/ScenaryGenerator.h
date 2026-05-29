#ifndef SCENGEN_H
#define SCENGEN_H

#include "Component.h"
#include "Rect_Vec2.h"
#include "Timer.h"
#include "SpriteRenderer.h"

class ScenaryGenerator : public Component
{
public:
    ScenaryGenerator(GameObject &associated,  Vec2 speed = {-30, 0}, Vec2 start = {1200, 600}, Vec2 end = {-120,0}, SDL_RendererFlip flip = SDL_FLIP_NONE, float angle = 0,int type = 0);
    ~ScenaryGenerator();
    void Update(float dt) override;
    void Render() override;
    bool Is(const std::string &type) override;
    void GenerateBatch(Vec2 Start, int pattern);
    int lastpattern = -1;
    Vec2 speed;
    Vec2 basestart;
    Vec2 baseend;
    Timer genTimer;
    static const int pattern[6][10];
    SDL_RendererFlip flip;
    float angle;
    GameObject* reference;
    int type = 0;
    int range = 0;
    float followrate = 1;
    int batchcount = 1;
};

#endif