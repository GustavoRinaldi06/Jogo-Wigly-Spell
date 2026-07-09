#include "../include/Transition.h"
#include "../include/Game.h"

Transition::Transition(GameObject &associated)
    : Component(associated), mode(TRAN_INTRO), currentFrame(0), frameTime(0.08f), timeElapsed(0.0f)
{
    sprite = new SpriteRenderer(associated, "recursos/img/Vinheta.png", 4, 3);
    sprite->SetCameraFollower(true);
    sprite->SetScale(1.0f, 1.0f);
    associated.AddComponent(sprite);


    if (sprite)
    {
        sprite->SetFrame(0, SDL_FLIP_NONE);
    }
}

void Transition::Update(float dt)
{
    if (mode == TRAN_IDLE || mode == TRAN_FINISHED)
        return;

    timeElapsed += dt;
    if (timeElapsed >= frameTime)
    {
        timeElapsed -= frameTime;

        // Intro
        if (mode == TRAN_INTRO)
        {
            currentFrame++;
            if (currentFrame > 8)
            {
                currentFrame = 8;
                mode = TRAN_IDLE; 
            }
        }
        // Outro
        else if (mode == TRAN_OUTRO)
        {
            currentFrame--;
            if (currentFrame < 0)
            {
                currentFrame = 0;
                mode = TRAN_FINISHED;
            }
        }

        if (sprite)
        {
            sprite->SetFrame(currentFrame, SDL_FLIP_NONE);
        }
    }
}

void Transition::Render() {}

bool Transition::Is(const std::string &type) { return type == "Transition"; }

void Transition::StartOutro()
{
    if (mode == TRAN_IDLE)
    {
        mode = TRAN_OUTRO;
        currentFrame = 8;
        timeElapsed = 0.0f;
    }
}

bool Transition::IsFinished() const { return mode == TRAN_FINISHED; }