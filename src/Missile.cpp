#include "Missile.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "State.h" 
#include "Camera.h"
#include "Collider.h"
#include "Game.h"
#include "GameData.h"
#include "Character.h"

#include <iostream>

Missile::Missile(GameObject &associated, const std::string &spritePath, int color)
    : Component(associated)
{
    associated.layer = 5.1;
    associated.blockable = 3;
    associated.damage = 1;
    
    auto renderer = new SpriteRenderer(associated, spritePath, 2, 6);
    associated.AddComponent(renderer);
    renderer->SetScale(1.5,2);
    associated.angleDeg = 270;

    // Novos sons
    //hitSound = Sound();
    //deathSound = Sound();
    //fallSound = Sound();

    // Cria as animações
    associated.color = color;
    auto animator = new Animator(associated);
    int base = 4 * associated.color;
    animator->AddAnimation("floating", Animation(base + 0,base + 1, 0.5f));
    animator->AddAnimation("dissipating", Animation(base + 2, base + 3, 0.25f));
    associated.AddComponent(animator);
    animator->SetAnimation("floating");

    associated.AddComponent(new Collider(associated,Vec2(0.8,0.8)));
    //associated.AddComponent(new Collider(associated,Vec2(1,1)));
    deathTimer.Restart();
    lifespan.Restart();
    LockinTimer.Restart();
}

Missile::~Missile()
{
}

void Missile::Start()
{}


void Missile::Update(float dt)
{
    // Ao morrer -------------------------------------------------------------------------------
    if (associated.box.x < -120) {
        associated.RequestDelete();
    }
    if (associated.color < 0) {
        destroyed = true;
    }
    if (destroyed)
    {
        // dispara animação e som apenas uma vez
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
        if (deathTimer.Get() > 0.5f)
            associated.RequestDelete();

        return; // não executa mais lógica de movimento
    }
    
    if (launched) {
        lifespan.Update(dt);
        if (lifespan.Get() > 15.0f) { 
            destroyed = true;
        }
        speed.x = -linearSpeed;
        speed.y = 0;
    }
    else {
        
        LockinTimer.Update(dt);
        if (LockinTimer.Get() > 0.5) {
            speed.x = 0;
            Rect playerbox = Character::PlayerBox();
            float target = playerbox.y + (playerbox.h/2 - associated.box.h/2);
            float dist = target - associated.box.y;
            if (abs(dist) < 5.0) {
                speed.y = 0;
            } 
            else if (dist > 0) {
                speed.y = 200;
            }
            else {
                speed.y = -200;
            }
            if (LockinTimer.Get() > 5.0) {
                launched = true;
        }
        }
    }
    associated.box.y += speed.y * dt;
    associated.box.x += speed.x * dt;
}

void Missile::Render() {}

bool Missile::Is(const std::string &type)
{
    return type == "Missile";
}


void Missile::NotifyCollision(GameObject &other)
{

    // Se colidir com chão
    Collider *collider = (Collider *)other.GetComponent("Collider");
    // Se colidir com parede
    if (launched && collider && (collider->tag == "solid")) 
    {
        // Destroi se colidir com parede
        destroyed = true;
    }
    else if (collider && collider->tag == "bullet") {
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

