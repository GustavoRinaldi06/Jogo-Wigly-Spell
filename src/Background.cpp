#include "Background.h"
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

Background::Background(GameObject &associated, const std::string &spritePath, int frame,SDL_RendererFlip flp, float layer, float followrate)
    : Component(associated)
{
    associated.layer = layer;
    auto renderer = new SpriteRenderer(associated, spritePath, 1, 1);
    //printf("%f",associated.box.w);
    associated.layer = layer;
    renderer->SetFrame(frame,flp);
    associated.AddComponent(renderer);
    
    limit = -associated.box.w;
    frate = followrate;
}

Background::~Background()
{
}

void Background::Start()
{}


void Background::Update(float dt)
{
    if (associated.box.x <= limit) {
        //associated.RequestDelete();
    }
    Vec2 uspeed = GameData::universalspeed;
    associated.box.x += (frate*uspeed.x) * dt;
    associated.box.y += (frate*uspeed.y)* dt;
}

void Background::Render() {}

bool Background::Is(const std::string &type)
{
    return type == "Background";
}
