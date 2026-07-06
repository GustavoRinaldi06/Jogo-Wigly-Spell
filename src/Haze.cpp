#include "Haze.h"
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

Haze::Haze(GameObject &associated, const std::string &spritePath, SDL_RendererFlip flp, float rotation)
    : Component(associated)
{
    associated.layer = 6.1;
    auto renderer = new SpriteRenderer(associated, spritePath, 1, 2);
    renderer->SetScale(2,3);
    associated.angleDeg = rotation;
    renderer->SetFrame(0,flp);
    associated.AddComponent(renderer);
    associated.damage = 1;
    // Cria as animações
     Collider *col = new Collider(associated,Vec2(1,1));
    //col->tag = "solid";
    associated.AddComponent(col);
    auto animator = new Animator(associated);
    animator->AddAnimation("idle", Animation(0, 1, 0.5f));
    associated.AddComponent(animator);
    animator->SetAnimation("idle");
    //associated.AddComponent(new Collider(associated,Vec2(1,1)));

}

Haze::~Haze()
{
}

void Haze::Start()
{}


void Haze::Update(float dt)
{
    
    if (associated.box.x <= limit) {
        associated.RequestDelete();
    }
    associated.box.x += (speed.x) * dt;
    associated.box.y += (speed.y)* dt;
}

void Haze::Render() {}

bool Haze::Is(const std::string &type)
{
    return type == "Haze";
}
