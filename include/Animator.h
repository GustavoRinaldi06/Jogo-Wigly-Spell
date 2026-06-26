#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <unordered_map>
#include <string>

#include "Component.h"
#include "Animation.h"
#include "GameObject.h"
#include "SpriteRenderer.h"

class Animator : public Component
{
public:
    Animator(GameObject &associated);

    void Update(float dt) override;
    void Render() override;
    bool Is(const std::string &type) override;

    void SetAnimation(const std::string &name);
    std::string &GetAnimation();
    void AddAnimation(const std::string &name, Animation anim);

    void Start() override;

    int GetCurrentFrame() const;
    void SetCurrentFrame(int frame);
    bool wrapped = false;

private:
    std::unordered_map<std::string, Animation> animations;

    int frameStart, frameEnd, currentFrame;
    float frameTime, timeElapsed;

    std::string current; // DEBUG
    SDL_RendererFlip flip;
};

#endif // ANIMATOR_H
