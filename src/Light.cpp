#include "../include/Light.h"
#include "../include/GameObject.h"
#include "../include/SpriteRenderer.h"
#include "../include/GameData.h"

Light::Light(GameObject &associated, const std::string &spritePath)
    : Component(associated)
{
    spriteRenderer = new SpriteRenderer(associated, spritePath, 2, 3);
    associated.AddComponent(spriteRenderer);

    // começa desligado
    spriteRenderer->SetFrame(5);
    int currentFrame = 5;

    turnOn = Sound("recursos/audio/light.wav");
}

Light::~Light() {}

void Light::Start() {}

void Light::Update(float dt)
{

    int nextFrame = 5;

    if (GameData::discoAttackActive)
    {
        if (GameData::discoError)
        {
            nextFrame = 4; // Holofote Vermelho de Erro
        }
        else if (GameData::discoBlackout)
        {
            nextFrame = 5; // Holofote Apagado
        }
        else
        {
            nextFrame = GameData::targetDiscoColor; // Cor do Boss
        }

        spriteRenderer->SetFrame(nextFrame);

        if (nextFrame != currentFrame && nextFrame != 5)
        {
            turnOn.Play(1);
            currentFrame = nextFrame;
        }
    }
    else
    {
        spriteRenderer->SetFrame(5);
    }
}

void Light::Render() {}

bool Light::Is(const std::string &type)
{
    return type == "Light";
}