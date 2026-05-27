#include "HomingProj.h"
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

HomingProj::HomingProj(GameObject &associated, const std::string &spritePath)
    : Component(associated)
{
    
    auto renderer = new SpriteRenderer(associated, spritePath, 2, 2);
    associated.AddComponent(renderer);
    renderer->SetScale(1.5,2);

    // Novos sons
    //hitSound = Sound();
    //deathSound = Sound();
    //fallSound = Sound();

    // Cria as animações
    auto animator = new Animator(associated);
    animator->AddAnimation("floating", Animation(0, 1, 0.5f));
    animator->AddAnimation("dissipating", Animation(2, 3, 0.25f));
    associated.AddComponent(animator);
    animator->SetAnimation("floating");

    associated.AddComponent(new Collider(associated,Vec2(0.8,0.8)));
    //associated.AddComponent(new Collider(associated,Vec2(1,1)));
    deathTimer.Restart();
    lifespan.Restart();
}

HomingProj::~HomingProj()
{
}

void HomingProj::Start()
{}


void HomingProj::Update(float dt)
{
    // Ao morrer -------------------------------------------------------------------------------
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

    lifespan.Update(dt);
    if (lifespan.Get() > 15.0f) { 
        destroyed = true;
    }

    // Enquanto funcional --------------------------------------------------------------------------

    // Aplica a gravidade --------------------------------------------------------------------
    float angle = ((Character::PlayerBox().GetCenter()-associated.box.GetCenter()).Angle());
    associated.angleDeg = 90 + (angle * 180)/M_PI;
    speed = Vec2(linearSpeed*cos(angle),linearSpeed*sin(angle));
    associated.box.y += speed.y * dt;
    associated.box.x += speed.x * dt;
}

void HomingProj::Render() {}

bool HomingProj::Is(const std::string &type)
{
    return type == "HomingProj";
}


void HomingProj::NotifyCollision(GameObject &other)
{

    // Se colidir com chão
    Collider *collider = (Collider *)other.GetComponent("Collider");
    Collider *col = (Collider *)associated.GetComponent("Collider");

    // Se colidir com parede
    if (collider && (collider->tag == "solid")) 
    {
        // Destroi se colidir com parede
        destroyed = true;
    }
}

