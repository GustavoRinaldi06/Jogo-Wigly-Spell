#include "Smoke.h"
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

Smoke::Smoke(GameObject &associated, const std::string &spritePath,  Vec2 pos, Vec2 scale, int type)
    : Component(associated)
{
    associated.layer = 6.0;
    SpriteRenderer *renderer;
    auto animator = new Animator(associated);
    if (type == 0) {
        renderer = new SpriteRenderer(associated, spritePath, 3, 2);
        renderer->SetScale(scale.x,scale.y);
        animator->AddAnimation("basic", Animation(0, 6, (1.0/12)));
        animator->SetAnimation("basic");
    }
    
    //printf("%f",associated.box.w);

    associated.AddComponent(renderer);
    associated.box.x = pos.x;
    associated.box.y = pos.y;
    associated.AddComponent(animator);
    

}

Smoke::~Smoke()
{
}

void Smoke::Start()
{}


void Smoke::Update(float dt)
{
    Animator *animator = static_cast<Animator *>(associated.GetComponent("Animator"));
    if (animator->wrapped) {
        associated.RequestDelete();
    }
}

void Smoke::Render() {}

bool Smoke::Is(const std::string &type)
{
    return type == "Smoke";
}

void Smoke::CreateSmoke(Vec2 pos, Vec2 scale, int type) {
    GameObject *GO = new GameObject();
    if (type == 0) {
    GO->AddComponent(new Smoke(*GO, "recursos/img/hit.png", pos,scale,type));

    }
    Game::GetInstance().GetCurrentState().AddObject(GO);
}
