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
#include "WavyNote.h"
#include "BeatWave.h"
#include "DanceGhost.h"
#include "Bullet.h"

#include <iostream>

CorridorGhost::CorridorGhost(GameObject &associated, const std::string &spritePath)
    : Component(associated)
{
    associated.layer = 4.5;
    associated.damage = 1;
  
    auto renderer = new SpriteRenderer(associated, spritePath, 12, 8);

    associated.AddComponent(renderer);

    renderer->SetScale(1,1);
    

    // Novos sons
    //hitSound = Sound();
    //deathSound = Sound();
    //fallSound = Sound();

    // Cria as animações

    auto animator = new Animator(associated);
    animator->AddAnimation("downgravprep", Animation(0, 9, (1.0/24)));
    animator->AddAnimation("downgrav", Animation(10, 15, (1.0/24)));
    animator->AddAnimation("downgravfin", Animation(16, 22,(1.0/24)));

    animator->AddAnimation("upgravprep", Animation(23, 34, (1.0/24)));
    animator->AddAnimation("upgrav", Animation(35, 40, (1.0/24)));
    animator->AddAnimation("upgravfin", Animation(41, 47,(1.0/24)));

    animator->AddAnimation("missileprep", Animation(61, 72, (1.0/24)));
    animator->AddAnimation("missile", Animation(73, 78, (1.0/24)));
    animator->AddAnimation("missilefin", Animation(79, 85,(1.0/24)));


    animator->AddAnimation("idle", Animation(49, 60, (1.0/24)));
    animator->AddAnimation("death", Animation(48, 48, 3.0f));
    associated.AddComponent(animator);
    animator->SetAnimation("idle");

    Collider *col = new Collider(associated,Vec2(Vec2(0.6,0.8)));
    associated.AddComponent(col);
    //associated.AddComponent(new Collider(associated,Vec2(1,1)));
    deathTimer.Restart();
    specialInvuln.Restart();
    health = 1000;
    dead = false;
    animTimer.Restart();

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
                    animator->SetAnimation("downgravprep");
                    attacked = false;
                }
                else {
                    animator->SetAnimation("upgravprep");
                    attacked = false;
                }
            }
            else {
                ATK = 1;
                animator->SetAnimation("missileprep");
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
        std::string current_anim = animator->GetAnimation();
        if (current_anim == "missileprep" && animator-> wrapped) {
            missHold = 1.0;
            animator->SetAnimation("missile");
            animTimer.Restart();
        }
        animTimer.Update(dt); 
        if (animTimer.Get() > missHold) {
            if (current_anim == "missile") {
                animTimer.Restart();
                animator->SetAnimation("missilefin");
            }
        }
        if (!attacked && animator->GetCurrentFrame() == 80) {
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
                        animator->SetCurrentFrame(10);
                        attacked = false;
                        animTimer.Restart();
                    }
                    else {
                        animator->SetAnimation("upgrav");
                        animator->SetCurrentFrame(35);
                        attacked = false;
                        animTimer.Restart();
                    }
                }
                else {
                    animator->SetCurrentFrame(73);
                    missHold = 0.6;
                    animator->SetAnimation("missile");
                    animTimer.Restart();
                }
                
            }
            
        }
        else if (animator->GetAnimation() == "missilefin" && animator->wrapped) {
            ATK = 0;
            
            animator->SetAnimation("idle");
        }
    }
    else if (ATK == 2) {
        std::string current_anim = animator->GetAnimation();
        if (animator->wrapped) {
            if (current_anim == "upgravprep") {
                animator->SetAnimation("upgrav");
                animTimer.Restart();
            }
            else if (current_anim == "downgravprep") {
                animator->SetAnimation("downgrav");
                animTimer.Restart();
            }
        }
        animTimer.Update(dt);
        if (animTimer.Get() > 3) {
            if (current_anim == "upgrav") {
                animTimer.Restart();
                animator->SetAnimation("upgravfin");
            }
            else if (current_anim == "downgrav") {
                animTimer.Restart();
                animator->SetAnimation("downgravfin");
            }
        }
        if (!attacked && !GameData::inverted && animator->GetCurrentFrame() == 42) {
            attacked = true;
            GameData::inverted = true;
        }
        if (!attacked && GameData::inverted && animator->GetCurrentFrame() == 17) {
            attacked = true;
            GameData::inverted = false;
        }

        if (animator->GetCurrentFrame() == 47 || animator->GetCurrentFrame() == 22) {
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
    //bounceGO->box.y = (rand() % 400) + 100;  // Centro do mapa
    //bounceGO->box.y = (rand() % 15)*60 + 50;  // Centro do mapa
    //bounceGO->box.y =  + 500;  // Centro do mapa 

    bounceGO->AddComponent(new BounceBall(*bounceGO, "recursos/img/bouncedisk.png")); // substitua pela imagem correta
    //bounceGO->AddComponent(new WavyNote(*bounceGO, "recursos/img/bounceB.png")); // substitua pela imagem correta
    //bounceGO->AddComponent(new BeatWave(*bounceGO, "recursos/img/BeatWave.png")); // substitua pela imagem correta
    //bounceGO->AddComponent(new DanceGhost(*bounceGO, "recursos/img/danceghost.png")); // substitua pela imagem correta
    Game::GetInstance().GetCurrentState().AddObject(bounceGO);
}
void CorridorGhost::MissileATK(int offset,int color) {
    GameObject *missileGO = new GameObject();
    missileGO->box.x = associated.box.x + offset;  // Centro do mapa
    missileGO->box.y = associated.box.y + associated.box.h/2; // Altura maior

    missileGO->AddComponent(new Missile(*missileGO, "recursos/img/garrafa.png",color)); // substitua pela imagem correta
    Game::GetInstance().GetCurrentState().AddObject(missileGO);
    
}

void CorridorGhost::NotifyCollision(GameObject &other)
{
    Collider *collider = (Collider *)other.GetComponent("Collider");
    if (collider && collider->tag == "bullet")
    {
        Bullet* bul = (Bullet *)other.GetComponent("Bullet");
        if (specialInvuln.Get() > 2.0 || bul->bulletcolor == 0 || bul->bulletcolor == 3) {
            
            health -= bul->damage;
            if (bul->bulletcolor == 0 || bul->bulletcolor == 3) {
                other.RequestDelete();
            }
            else {
                specialInvuln.Restart();
            }
        }
    }
}

