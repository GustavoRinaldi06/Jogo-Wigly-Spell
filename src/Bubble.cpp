#include "Bubble.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "State.h" 
#include "Camera.h"
#include "Collider.h"
#include "Game.h"
#include "GameData.h"
#include "Character.h"
#include "HallwayState.h"

#include <iostream>

Bubble::Bubble(GameObject &associated, const std::string &spritePath)
    : Component(associated)
{
    associated.layer = 5.2;
    auto renderer = new SpriteRenderer(associated, spritePath, 3, 1);
    //printf("%f",associated.box.w);
    renderer->SetFrame(0,SDL_FLIP_NONE);
    associated.AddComponent(renderer);
    associated.box.x = Character::PlayerBox().x;
    associated.box.y = Character::PlayerBox().y - 16;
}

Bubble::~Bubble()
{
}

void Bubble::Start()
{}


void Bubble::Update(float dt)
{
    associated.box.x = Character::PlayerBox().x;
    associated.box.y = Character::PlayerBox().y - 16;
}

void Bubble::Render() {}

bool Bubble::Is(const std::string &type)
{
    return type == "Bubble";
}
