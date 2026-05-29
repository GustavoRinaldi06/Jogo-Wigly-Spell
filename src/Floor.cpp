#include "Floor.h"
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

Floor::Floor(GameObject &associated, const std::string &spritePath, int frame,SDL_RendererFlip flp, float rotation)
    : Component(associated)
{
    associated.layer = 3.0;
    auto renderer = new SpriteRenderer(associated, spritePath, 2, 1);
    renderer->SetScale(4,2);
    associated.angleDeg = rotation;
    renderer->SetFrame(frame,flp);
    associated.AddComponent(renderer);
    
    // Cria as animações
    if (frame % 2 == 0) {
        Collider *col = new Collider(associated,Vec2(1,1));
        col->tag = "solid";
        associated.AddComponent(col);

    }
    
    //associated.AddComponent(new Collider(associated,Vec2(1,1)));

}

Floor::~Floor()
{
}

void Floor::Start()
{}


void Floor::Update(float dt)
{
    
    if (associated.box.x <= limit) {
        associated.RequestDelete();
    }
    Vec2 uspeed = GameData::universalspeed;
    associated.box.x += (speed.x + uspeed.x) * dt;
    associated.box.y += (speed.y + uspeed.y)* dt;
}

void Floor::Render() {}

bool Floor::Is(const std::string &type)
{
    return type == "Floor";
}
