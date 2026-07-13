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

    distance = 60;

    renderer->SetFrame(0,SDL_FLIP_NONE);
    associated.AddComponent(renderer);
    associated.box.x = Character::PlayerBox().x;
    associated.box.y = Character::PlayerBox().y;

     auto animator = new Animator(associated);
    animator->AddAnimation("idle", Animation(0, 3, (1.0/6)));
    animator->AddAnimation("damage", Animation(4, 5, (1.0/6)));
    animator->AddAnimation("dead", Animation(6, 7, (1.0/6)));
    animator->AddAnimation("attack", Animation(8, 9, (1.0/6)));
    animator->AddAnimation("wall", Animation(10, 11, (1.0/6)));
    animator->AddAnimation("walldamage", Animation(12, 13, (1.0/6)));

    animator->AddAnimation("idleP", Animation(0+16, 3+16, (1.0/6)));
    animator->AddAnimation("damageP", Animation(4+16, 5+16, (1.0/6)));
    animator->AddAnimation("deadP", Animation(6+16, 7+16, (1.0/6)));
    animator->AddAnimation("attackP", Animation(8+16, 9+16, (1.0/6)));
    animator->AddAnimation("wallP", Animation(10+16, 11+16, (1.0/6)));
    animator->AddAnimation("walldamageP", Animation(12+16, 13+16, (1.0/6)));

    animator->AddAnimation("none",Animation(14,14,10));
    associated.AddComponent(animator);
    animator->SetAnimation("idle");
    shotTime.Set(1000);

}

Cauldron::~Cauldron()
{
}

void Cauldron::Start()
{}


void Cauldron::Update(float dt)
{
    Vec2 mousepos = Vec2(InputManager::GetInstance().GetMouseX()+Camera::GetInstance().GetPosition().x,InputManager::GetInstance().GetMouseY()+Camera::GetInstance().GetPosition().y);


    Vec2 playercenter = Character::PlayerBox().GetCenter();
    Vec2 delta = mousepos - playercenter;
    //printf("%f %f %f %f\n",mousepos.x,mousepos.y,delta.x,delta.y);
    if (delta.Magnitude() < 0.01f)
    {
        delta = Vec2(1, 0);
    }
    Vec2 direction = delta.Normalize();

    if (GameData::shotFired) {
        GameData::shotFired = false;
        shotTime.Restart();
    }

    float angle = atan2(direction.y, direction.x);
    

    Animator *animator = static_cast<Animator *>(associated.GetComponent("Animator"));
    SpriteRenderer *renderer = static_cast<SpriteRenderer *>(associated.GetComponent("SpriteRenderer"));

    associated.box.x = playercenter.x -associated.box.w/2 + distance*direction.x;
    associated.box.y = playercenter.y -associated.box.h/2 + distance*direction.y;
    shotTime.Update(dt);
    float urpletime = Character::player->purpleTimer.Get();
    bool urple = urpletime < 30.0;
    
    if (Character::player->damageCooldown.Get() < 2.0) {
        renderer->SetTransparency(128);
    }
    else {
        renderer->SetTransparency(255);
    }
    
    renderer->SetScale(1.0,1.0);
    distance = 60;
    associated.angleDeg = 0;
    float anglemul = 1;
    if (GameData::inverted) {
        anglemul = -1;
        renderer->SetFrame(animator->GetCurrentFrame(),SDL_FLIP_VERTICAL);
    }
    else {
        renderer->SetFrame(animator->GetCurrentFrame(),SDL_FLIP_NONE);
    }
    if (GameData::gameMode == 0) {
        renderer->SetScale(0.6,0.6);
        distance = 40;
    }
    if (shotTime.Get() < 0.5) {
        if (anglemul < 0) {
            renderer->SetFrame(animator->GetCurrentFrame(),SDL_FLIP_HORIZONTAL);
        }
        else {
            renderer->SetFrame(animator->GetCurrentFrame(),SDL_FLIP_NONE);
        }

        associated.angleDeg = (angle * 180 / M_PI + 90); // Adicionar 90 para a bala ficar horizontal em relação a arma

        if (urple) {
            animator->SetAnimation("attackP");
        }
        else {
            animator->SetAnimation("attack");
        }
        
    }
    else {
        std::string anim = Playeranim->GetAnimation();
        if (anim == "damage") {
            if (urple) {
                animator->SetAnimation("damageP");
            }
            else {
                animator->SetAnimation("damage");
            }
        }
        else if (anim == "dead") {
            renderer->SetTransparency(255);
            if (urple) {
                animator->SetAnimation("deadP");
            }
            else {
                animator->SetAnimation("dead");
            }
        }
        else if (anim == "damageWall") {
            if (urple) {
                animator->SetAnimation("walldamageP");
            }
            else {
                animator->SetAnimation("walldamage");
            }
        }
        else if (anim == "gone") {
            animator->SetAnimation("none");

        }
        else {
            if (GameData::gameMode == 1){
                if (urple) {
                    animator->SetAnimation("idleP");
                }
                else {
                    animator->SetAnimation("idle");
                }
            }
            else {
                if (urple) {
                    animator->SetAnimation("wallP");
                }
                else {
                    animator->SetAnimation("wall");
                }
            }
        }
    }
}

void Cauldron::Render() {}

bool Cauldron::Is(const std::string &type)
{
    return type == "Cauldron";
}
