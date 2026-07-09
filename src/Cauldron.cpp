#include "Cauldron.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "State.h" 
#include "Camera.h"
#include "Collider.h"
#include "Game.h"
#include "GameData.h"
#include "Character.h"
#include "HallwayState.h"
#include "InputManager.h"

#include <iostream>

Cauldron::Cauldron(GameObject &associated, const std::string &spritePath,Animator *anim)
    : Component(associated)
{
    associated.layer = 5.1;
    auto renderer = new SpriteRenderer(associated, spritePath, 4, 8);

    Playeranim = anim;
    //printf("%f",associated.box.w);

    

    

    renderer->SetFrame(0,SDL_FLIP_NONE);
    associated.AddComponent(renderer);
    associated.box.x = Character::PlayerBox().x + 60;
    associated.box.y = Character::PlayerBox().y + (Character::PlayerBox().h - associated.box.h)/2.0;

     auto animator = new Animator(associated);
    animator->AddAnimation("idle", Animation(0, 3, (1.0/6)));
    animator->AddAnimation("damage", Animation(4, 5, (1.0/6)));
    animator->AddAnimation("dead", Animation(6, 7, (1.0/6)));
    animator->AddAnimation("attack", Animation(6, 7, (1.0/6)));
    animator->AddAnimation("none",Animation(14,14,10));
    associated.AddComponent(animator);
    animator->SetAnimation("idle");

}

Cauldron::~Cauldron()
{
}

void Cauldron::Start()
{}


void Cauldron::Update(float dt)
{
    Vec2 mousepos = Vec2(InputManager::GetInstance().GetMouseX(),InputManager::GetInstance().GetMouseY());

    Vec2 playercenter = Character::PlayerBox().GetCenter();
    Vec2 delta = mousepos - playercenter;

    if (delta.Magnitude() < 0.01f)
    {
        delta = Vec2(1, 0);
    }
    Vec2 direction = delta.Normalize();

    associated.box.x = playercenter.x -associated.box.w/2 + distance*direction.x;
    associated.box.y = playercenter.y -associated.box.h/2 + distance*direction.y;
}

void Cauldron::Render() {}

bool Cauldron::Is(const std::string &type)
{
    return type == "Cauldron";
}
