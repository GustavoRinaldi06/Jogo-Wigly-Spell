#include "BubbleCD.h"
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

BubbleCD::BubbleCD(GameObject &associated, const std::string &spritePath)
    : Component(associated)
{
    associated.layer = 5.3;
    auto renderer = new SpriteRenderer(associated, spritePath, 4, 2);
    //printf("%f",associated.box.w);

    renderer->SetFrame(0,SDL_FLIP_NONE);
    associated.AddComponent(renderer);
    associated.box.x = Character::PlayerBox().x;
    associated.box.y = Character::PlayerBox().y - (Character::PlayerBox().h - associated.box.h)/2.0;

    auto animator = new Animator(associated);
    animator->AddAnimation("none", Animation(7, 7, 10));
    animator->AddAnimation("countdown", Animation(0, 6, 9.0/7));
    associated.AddComponent(animator);
    animator->SetAnimation("none");

}

BubbleCD::~BubbleCD()
{
}

void BubbleCD::Start()
{}


void BubbleCD::Update(float dt)
{
    associated.box.x = Character::PlayerBox().x;
    associated.box.y = Character::PlayerBox().y + (Character::PlayerBox().h - associated.box.h)/2.0 - 32;
    Animator *animator = static_cast<Animator *>(associated.GetComponent("Animator"));
    SpriteRenderer *rend = static_cast<SpriteRenderer *>(associated.GetComponent("SpriteRenderer"));
    if (animator->GetCurrentFrame() == 6) {
        animator->SetAnimation("none");
    }
    if (GameData::inverted) {
        rend->SetFrame(animator->GetCurrentFrame(),SDL_FLIP_VERTICAL);
    }
    else {
        rend->SetFrame(animator->GetCurrentFrame(),SDL_FLIP_NONE);
    }
}

void BubbleCD::Render() {}

bool BubbleCD::Is(const std::string &type)
{
    return type == "BubbleCD";
}
