#include "../include/Animator.h"

Animator::Animator(GameObject &associated)
    : Component(associated), frameStart(0), frameEnd(0), currentFrame(0), frameTime(0), timeElapsed(0) {}

void Animator::Update(float dt)
{
    // Atualizei a animação baseado em dt
    if (frameTime != 0)
    {
        timeElapsed += dt;

        if (timeElapsed > frameTime)
        {
            currentFrame++;
            timeElapsed -= frameTime;

            if (currentFrame > frameEnd)
            {
                    currentFrame = frameStart; // Loop normal para outras animações
            }

            // Atualiza o frame do SpriteRenderer
            SpriteRenderer *sprite = (SpriteRenderer *)associated.GetComponent("SpriteRenderer");
            if (sprite)
            {
                sprite->SetFrame(currentFrame, flip);
            }
        }
    }
}

void Animator::Render()
{
    // Vazio
}

bool Animator::Is(const std::string &type)
{
    return type == "Animator";
}

void Animator::SetAnimation(const std::string &name)
{
    if (current == name) return; // evita resetar animação todo frame

    auto setA = animations.find(name);
    if (setA != animations.end()) // Se achou a animação
    {
        Animation anim = setA ->second;
        // Atualiza valores de animção
        frameStart = anim.frameStart;
        frameEnd = anim.frameEnd;
        frameTime = anim.frameTime;
        currentFrame = frameStart;
        timeElapsed = 0;
        flip = anim.flip;
        current = name; // nome da animação atual para não alterar

        // Atualiza o SpriteRenderer para o frame inicial
        SpriteRenderer *sprite = (SpriteRenderer *)associated.GetComponent("SpriteRenderer");
        if (sprite)
        {
            sprite->SetFrame(currentFrame, flip);
        }
    }
}

void Animator::AddAnimation(const std::string &name, Animation anim) // FAcilitador para modificar animações no futuro
{
    if (animations.find(name) == animations.end()) // Caso não ache a chave, acidiona ela
    {
        animations.emplace(name, anim);
    }
}

void Animator::Start()
{}

int Animator::GetCurrentFrame() const // DEBUG
{
    return currentFrame;
}
