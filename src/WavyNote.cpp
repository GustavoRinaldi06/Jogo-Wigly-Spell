#include "WavyNote.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "State.h" 
#include "Camera.h"
#include "Collider.h"
#include "Game.h"
#include "GameData.h"
#include "HallwayState.h"

#include <iostream>

WavyNote::WavyNote(GameObject &associated, const std::string &spritePath)
    : Component(associated)
{
    associated.layer = 5.1;
    associated.blockable = 3;
    associated.damage = 1;
    auto renderer = new SpriteRenderer(associated, spritePath, 2, 4);
    associated.AddComponent(renderer);
    renderer->SetScale(1,1);
    

    // Novos sons
    //hitSound = Sound();
    //deathSound = Sound();
    //fallSound = Sound();

    // Cria as animações
    auto animator = new Animator(associated);
    animator->AddAnimation("floating", Animation(0, 1, 0.25f));
    animator->AddAnimation("bouncing", Animation(2, 3, 0.2f));
    animator->AddAnimation("above", Animation(4, 5, 0.75f));
    animator->AddAnimation("popping", Animation(6, 7, 0.25f));
    associated.AddComponent(animator);
    animator->SetAnimation("floating");

    associated.AddComponent(new Collider(associated,Vec2(0.8,0.8)));
    //associated.AddComponent(new Collider(associated,Vec2(1,1)));
    deathTimer.Restart();
    lifespan.Restart();
    sinval = 0;
    speed.x = -50;
}

WavyNote::~WavyNote()
{
}

void WavyNote::Start()
{}


void WavyNote::Update(float dt)
{
    // Ao morrer -------------------------------------------------------------------------------
    if (associated.box.y < -120 || associated.box.y > 1400 || associated.box.x < -120) {
         associated.RequestDelete();
    }
    
    if (destroyed)
    {
        // dispara animação e som apenas uma vez

        associated.damage = -1;
        if (!deathAnimTriggered)
        {
            deathAnimTriggered = true;
            fallSound.Play(1);

            // seta animação "popping"
            Animator *animator = static_cast<Animator *>(associated.GetComponent("Animator"));
            if (animator)
                animator->SetAnimation("popping");
            deathTimer.Restart();
        }

        // avança timer de morte
        deathTimer.Update(dt);

        // só deleta após 0.5s
        if (deathTimer.Get() > 0.5f)
            associated.RequestDelete();

        return; // não executa mais lógica de movimento
    }

    lifespan.Update(dt);
    if (lifespan.Get() > 50.0f) { 
        destroyed = true;
    }

    // Enquanto funcional --------------------------------------------------------------------------

    sinval += dt;

    speed.y = 150*cos(1.5*sinval);
        
    Vec2 uspeed = GameData::universalspeed;
    uspeed = Vec2(0,0);
    associated.box.x += (speed.x + uspeed.x) * dt;
    associated.box.y += (speed.y + uspeed.y)* dt;
    
    // Atualiza animação de acordo com a movimentação
    Animator *animator = static_cast<Animator *>(associated.GetComponent("Animator"));
    //SpriteRenderer *renderer = static_cast<SpriteRenderer *>(associated.GetComponent("SpriteRenderer"));
    if (animator)
    {
        animator->Update(dt);
        animator->SetAnimation("floating");         
    }
}

void WavyNote::Render() {}

bool WavyNote::Is(const std::string &type)
{
    return type == "WavyNote";
}


void WavyNote::NotifyCollision(GameObject &other)
{
}

