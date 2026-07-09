#include "DanceGhost.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "State.h" 
#include "Camera.h"
#include "Collider.h"
#include "Game.h"
#include "GameData.h"
#include "HallwayState.h"
#include "StarProj.h"
#include "WavyNote.h"
#include "BeatWave.h"
#include "Bullet.h"
#include "Smoke.h"

#include <iostream>

DanceGhost::DanceGhost(GameObject &associated, const std::string &spritePath, int position)
    : Component(associated)
{
    associated.layer = 4.7;
    associated.damage = 1;
  
    auto renderer = new SpriteRenderer(associated, spritePath, 8, 6);

    associated.AddComponent(renderer);

    renderer->SetScale(1,1);
    

    // Novos sons
    //hitSound = Sound();
    //deathSound = Sound();
    //fallSound = Sound();

    // Cria as animações

    auto animator = new Animator(associated);
    animator->AddAnimation("rush", Animation(24, 27, (1.0/24)));
    animator->AddAnimation("idle", Animation(16, 23, (1.0/24)));
    animator->AddAnimation("attack", Animation(28, 45, (1.0/24)));
    animator->AddAnimation("death", Animation(0, 15, (1.0/24)));
    associated.AddComponent(animator);
    animator->SetAnimation("rush");

    Collider *col = new Collider(associated,Vec2(Vec2(0.8,0.8)));
    associated.AddComponent(col);
    //associated.AddComponent(new Collider(associated,Vec2(1,1)));
    deathTimer.Restart();
    specialInvuln.Restart();
    health = 50;
    dead = false;
    pos = position;
    attacked = false;
}

DanceGhost::~DanceGhost()
{
}

void DanceGhost::Start()
{}


void DanceGhost::Update(float dt)
{
    SpriteRenderer *renderer = static_cast<SpriteRenderer *>(associated.GetComponent("SpriteRenderer"));
    if (health <= 0) {
        dead = true;
    }
    if (GameData::finalfase) {
        dead = true;
    }
    
    Animator *animator = static_cast<Animator *>(associated.GetComponent("Animator"));
    Collider *collider = static_cast<Collider *>(associated.GetComponent("Collider"));
    // Ao morrer -------------------------------------------------------------------------------
    if (dead)
    { 
        // dispara animação e som apenas uma vez
        associated.damage = -1;
        if (pos >= 0) {
            GameData::summonalive[pos] = false;
        }
        if (!deathAnimTriggered)
        {
            deathAnimTriggered = true;
            //fallSound.Play(1);

            // seta animação "popping"
            Animator *animator = static_cast<Animator *>(associated.GetComponent("Animator"));
            if (animator)
                animator->SetAnimation("death");
            deathTimer.Restart();
        }

        // avança timer de morte
        deathTimer.Update(dt);

        // só deleta após 0.5s
        if (animator->GetCurrentFrame() == 15)
            
            associated.RequestDelete();

        return; // não executa mais lógica de movimento
    }
    
    
    
    
    specialInvuln.Update(dt);
    AtkTimer.Update(dt);
    if (rushing) {
        AtkTimer.Restart();
        speed.x = -400;
        speed.y = 0;
        collider->SetScale(Vec2(0.8,0.6));
        if (associated.box.x < 100) {
            rushing = false;
            collider->SetScale(Vec2(0.6,0.8));
            animator->SetAnimation("idle");
        }
    }
    else {
        
        speed.x = 0;
        speed.y = 0;
        if (!attacking && AtkTimer.Get() > startime) {
            if (!attacking) {
                AtkTimer.Restart();
                animator->SetAnimation("attack");
                attacking = true;
                attacked = false;
            }   
        }
        if (attacking) {
            
            if (animator->GetCurrentFrame() == 40 && !attacked) {
                attacked = true; 
                StarATK();
            }

            else if (attacked && animator->wrapped) {
                animator->SetAnimation("idle");
                attacking = false;
                attacked = false;
                AtkTimer.Restart();
            }
        }

    }

    associated.box.x += (speed.x) * dt;
    associated.box.y += (speed.y)* dt;
    if (!rushing) {
        renderer->SetFrame(animator->GetCurrentFrame(),SDL_FLIP_HORIZONTAL);
    }

}

void DanceGhost::Render() {}

bool DanceGhost::Is(const std::string &type)
{
    return type == "DanceGhost";
}


void DanceGhost::StarATK() {
    GameObject *starGO = new GameObject();
    starGO->box.x = associated.box.x + + 3*associated.box.w/4;  // Centro do mapa
    starGO->box.y = associated.box.y + 3*associated.box.h/5; // Altura maior

    starGO->AddComponent(new StarProj(*starGO, "recursos/img/miniatk.png")); // substitua pela imagem correta
    Game::GetInstance().GetCurrentState().AddObject(starGO);
    
}

void DanceGhost::NotifyCollision(GameObject &other)
{
    Collider *collider = (Collider *)other.GetComponent("Collider");
    if (collider && collider->tag == "bullet")
    {
        Bullet* bul = (Bullet *)other.GetComponent("Bullet");
        if (specialInvuln.Get() > 2.0 || bul->bulletcolor == 0 || bul->bulletcolor == 3) {
            
            health -= bul->damage;
            if (bul->bulletcolor == 0 || bul->bulletcolor == 3) {
                other.StandardSmoke();
                other.RequestDelete();
            }
            else {
                specialInvuln.Restart();
            }
        }
        
    }
}

