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

#include <iostream>

DanceGhost::DanceGhost(GameObject &associated, const std::string &spritePath)
    : Component(associated)
{
    associated.layer = 4.7;
    associated.damage = 1;
  
    auto renderer = new SpriteRenderer(associated, spritePath, 2, 6);

    associated.AddComponent(renderer);

    renderer->SetScale(1.5,1.5);
    

    // Novos sons
    //hitSound = Sound();
    //deathSound = Sound();
    //fallSound = Sound();

    // Cria as animações

    auto animator = new Animator(associated);
    animator->AddAnimation("rush", Animation(0, 1, 0.15f));
    animator->AddAnimation("idle", Animation(2, 3, 0.4f));
    animator->AddAnimation("attack", Animation(4, 5, 0.4f));
    animator->AddAnimation("death", Animation(6, 11, 0.1f));
    associated.AddComponent(animator);
    animator->SetAnimation("rush");

    Collider *col = new Collider(associated,Vec2(Vec2(0.8,0.8)));
    associated.AddComponent(col);
    //associated.AddComponent(new Collider(associated,Vec2(1,1)));
    deathTimer.Restart();
    specialInvuln.Restart();
    health = 50;
    dead = false;

}

DanceGhost::~DanceGhost()
{
}

void DanceGhost::Start()
{}


void DanceGhost::Update(float dt)
{
    if (health <= 0) {
        dead = true;
    }
    // Ao morrer -------------------------------------------------------------------------------
    if (dead)
    { 
        // dispara animação e som apenas uma vez
        associated.damage = -1;
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
        if (deathTimer.Get() > 0.55f)
            associated.RequestDelete();

        return; // não executa mais lógica de movimento
    }
    
    Animator *animator = static_cast<Animator *>(associated.GetComponent("Animator"));
    
    specialInvuln.Update(dt);
    AtkTimer.Update(dt);
    if (rushing) {
        AtkTimer.Restart();
        speed.x = -400;
        speed.y = 0;
        associated.angleDeg = -90;
        if (associated.box.x < 100) {
            rushing = false;
            animator->SetAnimation("idle");
        }
    }
    else {
        associated.angleDeg = 0;
        speed.x = 0;
        speed.y = 0;
        if (AtkTimer.Get() > startime) {
            if (!attacking) {
                AtkTimer.Restart();
                animator->SetAnimation("attack");
                attacking = true;
            }
            else {
                StarATK();
                AtkTimer.Restart();
                animator->SetAnimation("idle");
                attacking = false;
            }
            
        }
    }

    associated.box.x += (speed.x) * dt;
    associated.box.y += (speed.y)* dt;
}

void DanceGhost::Render() {}

bool DanceGhost::Is(const std::string &type)
{
    return type == "DanceGhost";
}


void DanceGhost::StarATK() {
    GameObject *starGO = new GameObject();
    starGO->box.x = associated.box.x ;  // Centro do mapa
    starGO->box.y = associated.box.y + 3*associated.box.h/4; // Altura maior

    starGO->AddComponent(new StarProj(*starGO, "recursos/img/starProj.png")); // substitua pela imagem correta
    Game::GetInstance().GetCurrentState().AddObject(starGO);
    
}

void DanceGhost::NotifyCollision(GameObject &other)
{
    Collider *collider = (Collider *)other.GetComponent("Collider");
    if (collider && collider->tag == "bullet" && specialInvuln.Get() > 2.0)
    {
        
        Bullet* bul = (Bullet *)other.GetComponent("Bullet");
        health -= bul->damage;
        if (bul->bulletcolor == 0 || bul->bulletcolor == 3) {
            other.RequestDelete();
        }
        else {
            specialInvuln.Restart();
        }
    }
}

