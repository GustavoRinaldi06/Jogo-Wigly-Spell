#include "Health.h"
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

Health::Health(GameObject &associated, const std::string &spritePath,int id)
    : Component(associated)
{
    basey = associated.box.y;
    auto renderer = new SpriteRenderer(associated, spritePath, 4, 2);
    renderer->SetFrame(4);
    renderer->SetCameraFollower(1.0);
    renderer->SetScale(0.8,0.8);
    
    associated.AddComponent(renderer);
    number=id;
    
}

Health::~Health()
{
}

void Health::Start()
{}


void Health::Update(float dt)
{
    SpriteRenderer *renderer = (SpriteRenderer *)associated.GetComponent("SpriteRenderer");
    int health = GameData::playerHP;
    if (int(health / 20) > number) {
        associated.box.y = basey;
    }
    else {
        associated.box.y = 2000;
    }
}

void Health::Render() {}

bool Health::Is(const std::string &type)
{
    return type == "Health";
}
