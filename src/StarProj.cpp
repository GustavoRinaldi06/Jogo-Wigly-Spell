#include "StarProj.h"
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

StarProj::StarProj(GameObject &associated, const std::string &spritePath)
    : Component(associated)
{
    associated.layer = 5.1;
    associated.blockable = 1;
    associated.damage = 1;
    
    auto renderer = new SpriteRenderer(associated, spritePath, 4, 2);
    associated.AddComponent(renderer);
    renderer->SetScale(1.0,1.0);

    // Novos sons
    //hitSound = Sound();
    //deathSound = Sound();
    //fallSound = Sound();

    // Cria as animações
    auto animator = new Animator(associated);
    animator->AddAnimation("floating", Animation(0, 4, (1.0/24)));
    animator->AddAnimation("dissipating", Animation(4, 7, 0.25f));
    associated.AddComponent(animator);
    animator->SetAnimation("floating");

    associated.AddComponent(new Collider(associated,Vec2(0.5,0.5)));
    //associated.AddComponent(new Collider(associated,Vec2(1,1)));
    angle = ((Character::PlayerBox().GetCenter()-associated.box.GetCenter()).Angle());
    
    deathTimer.Restart();
    lifespan.Restart();
    linearSpeed = 200;
}

StarProj::~StarProj()
{
}

void StarProj::Start()
{}


void StarProj::Update(float dt)
{
    if (associated.color < 0) {
        destroyed = true;
    }
    
    // Ao morrer -------------------------------------------------------------------------------
    if (destroyed)
    {
        // dispara animação e som apenas uma vez
        associated.damage = 0;
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

    lifespan.Update(dt);
    if (lifespan.Get() > 10.0f) { 
        destroyed = true;
    }

    // Enquanto funcional --------------------------------------------------------------------------

    // Aplica a gravidade --------------------------------------------------------------------
    speed = Vec2(linearSpeed*cos(angle),linearSpeed*sin(angle));
    associated.box.y += speed.y * dt;
    associated.box.x += speed.x * dt;
}

void StarProj::Render() {}

bool StarProj::Is(const std::string &type)
{
    return type == "StarProj";
}


void StarProj::NotifyCollision(GameObject &other)
{

    // Se colidir com chão
    Collider *collider = (Collider *)other.GetComponent("Collider");
    //Collider *col = (Collider *)associated.GetComponent("Collider");

    // Se colidir com parede
    if (collider && (collider->tag == "solid")) 
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

