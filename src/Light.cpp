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
}

Light::~Light() {}

void Light::Start() {}

void Light::Update(float dt)
{
    if (GameData::discoAttackActive)
    {
        if (GameData::discoError)
        {
            spriteRenderer->SetFrame(4); // Holofote Vermelho de Erro
        }
        else if (GameData::discoBlackout)
        {
            spriteRenderer->SetFrame(5); // Holofote Apagado
        }
        else
        {
            spriteRenderer->SetFrame(GameData::targetDiscoColor); // Cor do Boss
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