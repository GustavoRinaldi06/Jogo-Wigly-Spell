#pragma once
#include "Component.h"
#include "SpriteRenderer.h"
#include "Timer.h"

enum TransitionMode
{
    TRAN_INTRO,
    TRAN_IDLE,
    TRAN_OUTRO,
    TRAN_FINISHED
};

class Transition : public Component
{
private:
    TransitionMode mode;
    int currentFrame;
    float frameTime;
    float timeElapsed;
    SpriteRenderer *sprite;

public:
    Transition(GameObject &associated);
    void Update(float dt) override;
    void Render() override;
    bool Is(const std::string &type) override;

    void StartOutro();    
    bool IsFinished() const; 
};