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

WavyNote::WavyNote(GameObject &associated, const std::string &spritePath, int color)
    : Component(associated)
{
    associated.layer = 5.1;
    associated.blockable = 3;
    associated.damage = 1;
    auto renderer = new SpriteRenderer(associated, spritePath, 4, 3);
    associated.AddComponent(renderer);
    renderer->SetScale(1.5,1.5);
    

    // Novos sons
    //hitSound = Sound();
    //deathSound = Sound();
    //fallSound = Sound();

    // Cria as animações
    auto animator = new Animator(associated);
    int base = 0;
    if (color == 1) {
        base = 8;
    }
    else if (color == 2) {
        base = 4;
    }
    animator->AddAnimation("floating", Animation(base,base + 3, 0.25f));
    associated.AddComponent(animator);
    animator->SetAnimation("floating");

    associated.AddComponent(new Collider(associated,Vec2(0.8,0.8)));
    //associated.AddComponent(new Collider(associated,Vec2(1,1)));
    deathTimer.Restart();
    lifespan.Restart();
    sinval = 0;
    speed.x = -50;
    associated.color = color;
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

    if (associated.color < 0) {
        destroyed = true;
    }
    if (GameData::finalfase) {
        destroyed = true;
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
                animator->SetAnimation("dissipating");
            deathTimer.Restart();
        }

        // avança timer de morte
        deathTimer.Update(dt);

        // só deleta após 0.5s
        if (deathTimer.Get() > 0.15f)
            associated.RequestDelete();

        return; // não executa mais lógica de movimento
    }

    lifespan.Update(dt);
    if (lifespan.Get() > 50.0f) { 
        destroyed = true;
    }

    // Enquanto funcional --------------------------------------------------------------------------

    sinval += dt;
    if (GameData::bossHP > 900) {
        speed.x = -50;
    }
    else {
        speed.x = -100;
    }

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
    Collider *collider = (Collider *)other.GetComponent("Collider");
    if (collider && collider->tag == "bullet") {
        Bullet* bul = (Bullet *)other.GetComponent("Bullet");
        if (bul->bulletcolor == 3 && bul->purpledurability > 0 && blockable > 0) {
            int block = blockable;
            blockable -= bul->purpledurability;
            bul->purpledurability -= block;
            if (blockable <= 0) {
                destroyed = true; 
            }
            
        }
    }
}

