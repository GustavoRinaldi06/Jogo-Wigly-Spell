#include "Potion.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "State.h" 
#include "Camera.h"
#include "Collider.h"
#include "Game.h"
#include "GameData.h"
#include "Character.h"
#include "StageState.h"

#include <iostream>

Potion::Potion(GameObject &associated, const std::string &spritePath,int num)
    : Component(associated)
{
    auto renderer = new SpriteRenderer(associated, spritePath, 3, 1);
    renderer->SetFrame(0,SDL_FLIP_NONE);
    associated.AddComponent(renderer);
    number=num;
}

Potion::~Potion()
{
}

void Potion::Start()
{}


void Potion::Update(float dt)
{
    
    SpriteRenderer *renderer = (SpriteRenderer *)associated.GetComponent("SpriteRenderer");
    try {
         renderer->SetFrame(static_cast<int>((Character::player->GetColorInventory()).at(number)),SDL_FLIP_NONE);
    }
    catch (...) {
        renderer->SetFrame(0,SDL_FLIP_NONE);

    }
}

void Potion::Render() {}

bool Potion::Is(const std::string &type)
{
    return type == "Potion";
}
