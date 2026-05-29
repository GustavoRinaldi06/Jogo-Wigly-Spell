#include "CorrGhost.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "State.h" 
#include "Camera.h"
#include "Collider.h"
#include "Game.h"
#include "GameData.h"
#include "HallwayState.h"
#include "Missile.h"
#include "bounceBall.h"
#include "Bullet.h"

#include <iostream>

CorridorGhost::CorridorGhost(GameObject &associated, const std::string &spritePath)
    : Component(associated)
{
    associated.layer = 4.5;
    associated.damage = 1;
  
    auto renderer = new SpriteRenderer(associated, spritePath, 6, 14);

    associated.AddComponent(renderer);

    renderer->SetScale(1.5,1.5);
    

    // Novos sons
    //hitSound = Sound();
    //deathSound = Sound();
    //fallSound = Sound();

    // Cria as animações

    auto animator = new Animator(associated);
    animator->AddAnimation("downgrav", Animation(0, 23, 0.2f));
    animator->AddAnimation("upgrav", Animation(24, 47, 0.2f));
    animator->AddAnimation("missile", Animation(48, 71, 0.1f));
    animator->AddAnimation("idle", Animation(72, 77, 0.15f));
    animator->AddAnimation("death", Animation(78, 78, 3.0f));
    associated.AddComponent(animator);
    animator->SetAnimation("idle");

    Collider *col = new Collider(associated,Vec2(Vec2(0.6,0.8)));
    associated.AddComponent(col);
    //associated.AddComponent(new Collider(associated,Vec2(1,1)));
    deathTimer.Restart();
    specialInvuln.Restart();
    health = 1000;
    dead = false;

}

CorridorGhost::~CorridorGhost()
{
}

void CorridorGhost::Start()
{}


void CorridorGhost::Update(float dt)
{
    GameData::bossHP = health;
    if (health <= 0) {
        dead = true;
    }
    // Ao morrer -------------------------------------------------------------------------------
    if (dead)
    { 
        // dispara animação e som apenas uma vez
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
        if (deathTimer.Get() > 3.0f)
            associated.RequestDelete();

        return; // não executa mais lógica de movimento
    }
    
    Animator *animator = static_cast<Animator *>(associated.GetComponent("Animator"));
    BounceTimer.Update(dt);
    specialInvuln.Update(dt);
    if (BounceTimer.Get() > bouncetime) {
        BounceTimer.Restart();
        bouncetime = 4 + rand() % 3;
        int count = 1;
        if (health < 500) {
            int random = rand() % 6;
            if (random >= 5) {
                count += 2;
            }
            else if (random > 2) {
                count += 1;
            }
        }
        for (int i = 0; i < count; i++)
            BounceATK(i * 50);
    }
    if (ATK == 0) {
        MissSwap.Update(dt);
        if (MissSwap.Get() > missTime) {
            MissSwap.Restart();
            missTime = 4 + rand() % 4;
            swapcount -= 1;
            
            if (swapcount == 0) {
                swapcount = 2 + (rand() % 3);
                ATK = 2;
                if  (GameData::inverted) {
                    animator->SetAnimation("downgrav");
                    attacked = false;
                }
                else {
                    animator->SetAnimation("upgrav");
                    attacked = false;
                }
            }
            else {
                ATK = 1;
                animator->SetAnimation("missile");
                misstotal = 0;
                misscount = 1;
                if (health < 500) {
                    misscount = 3;
                }
                colorsmnd = false;
                attacked = false;
            }
        }
    }
    else if (ATK == 1) {
        if (!attacked && animator->GetCurrentFrame() == 68) {
            int colorchk = rand() % 4;
            if (misstotal == 0 && misscount == 1 && colorchk < 2) {
                colorchk += 2;   // Sempre colorido se na primeira fase
            }
            int color = 0;
            if (colorchk == 2 && !colorsmnd) {
                color = 1;
                colorsmnd = true;
            }
            else if (colorchk == 3 && !colorsmnd) {
                color = 2;
                colorsmnd = true;
            }
            MissileATK(misstotal*40,color);
            misscount -=1;
            misstotal +=1;
            if (misscount == 0) {
                attacked = true;
            }
            else {
                if ((rand() % 4) == 0) {
                    misscount = 0;
                    swapcount = 2 + (rand() % 3);
                    ATK = 2;
                    if  (GameData::inverted) {
                        animator->SetAnimation("downgrav");
                        animator->SetCurrentFrame(3);
                        attacked = false;
                    }
                    else {
                        animator->SetAnimation("upgrav");
                        animator->SetCurrentFrame(27);
                        attacked = false;
                    }
                }
                else {
                    animator->SetCurrentFrame(63);
                }
                
            }
            
        }
        else if (animator->GetCurrentFrame() == 71) {
            ATK = 0;
            animator->SetAnimation("idle");
        }
    }
    else if (ATK == 2) {
        if (!attacked && !GameData::inverted && animator->GetCurrentFrame() == 44) {
            attacked = true;
            GameData::inverted = true;
        }
        if (!attacked && GameData::inverted && animator->GetCurrentFrame() == 20) {
            attacked = true;
            GameData::inverted = false;
        }

        if (animator->GetCurrentFrame() == 47 || animator->GetCurrentFrame() == 23) {
            ATK = 0;
            animator->SetAnimation("idle");
        }
    }
    if (health < 500) {
        GameData::universalspeed = Vec2(-100,0);
    }
}

void CorridorGhost::Render() {}

bool CorridorGhost::Is(const std::string &type)
{
    return type == "CorridorGhost";
}

void CorridorGhost::BounceATK(int offset) {
    GameObject *bounceGO = new GameObject();
    bounceGO->box.x = associated.box.x + associated.box.w + (float) offset;  // Centro do mapa
    bounceGO->box.y = associated.box.y + associated.box.h/2; // Altura maior

    bounceGO->AddComponent(new BounceBall(*bounceGO, "recursos/img/bounceB.png")); // substitua pela imagem correta
    Game::GetInstance().GetCurrentState().AddObject(bounceGO);
}
void CorridorGhost::MissileATK(int offset,int color) {
    GameObject *missileGO = new GameObject();
    missileGO->box.x = associated.box.x + offset;  // Centro do mapa
    missileGO->box.y = associated.box.y + associated.box.h/2; // Altura maior

    missileGO->AddComponent(new Missile(*missileGO, "recursos/img/homingProj.png",color)); // substitua pela imagem correta
    Game::GetInstance().GetCurrentState().AddObject(missileGO);
    
}

void CorridorGhost::NotifyCollision(GameObject &other)
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

