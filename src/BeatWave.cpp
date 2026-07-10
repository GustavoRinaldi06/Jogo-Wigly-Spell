#include "BeatWave.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "State.h" 
#include "Camera.h"
#include "Collider.h"
#include "Game.h"
#include "GameData.h"
#include "Character.h"
#include "HallwayState.h"

#include <iostream>

BeatWave::BeatWave(GameObject &associated, const std::string &spritePath, int argside)
    : Component(associated)
{
    associated.layer = 3.1;
    associated.blockable = -1;
    associated.damage = 1;
    auto renderer = new SpriteRenderer(associated, spritePath, 4, 1);
    associated.AddComponent(renderer);
    renderer->SetScale(1.65,1.65);
    

    // Novos sons
    //hitSound = Sound();
    //deathSound = Sound();
    //fallSound = Sound();

    // Cria as animações
    auto animator = new Animator(associated);
    animator->AddAnimation("flowing", Animation(0, 3, (1.0/6)));
    associated.AddComponent(animator);
    animator->SetAnimation("flowing");

    associated.AddComponent(new Collider(associated,Vec2(0.7,0.7)));
    //associated.AddComponent(new Collider(associated,Vec2(1,1)));
    deathTimer.Restart();
    lifespan.Restart();
    speed.x = -200;
    associated.damage = -1;
    if (argside > 0) {
        renderer->SetFrame(0,SDL_FLIP_VERTICAL);
        if (GameData::easy) {
            associated.box.y = -50;
        }
        else {
            associated.box.y = -10;
        }
        
    }
    else {
        renderer->SetFrame(0,SDL_FLIP_NONE);
    }
    side = argside;
}

BeatWave::~BeatWave()
{
}

void BeatWave::Start()
{}


void BeatWave::Update(float dt)
{
    // Ao morrer -------------------------------------------------------------------------------
    if (associated.box.y < -120 || associated.box.y > 1400 || associated.box.x < -240) {
         associated.RequestDelete();
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

    Vec2 playerdot = Character::player->PlayerBox().GetCenter();
    if (playerdot.x > associated.box.x && playerdot.x < associated.box.x + associated.box.w && playerdot.y > associated.box.y && playerdot.y < associated.box.y + associated.box.h) {
        float playerx = (playerdot.x - associated.box.x);
        float playery = associated.box.h - (playerdot.y - associated.box.y);
        float leeway =0.85;
        float ileeway = 1 - leeway;
        if (playery < leeway*associated.box.h*sin((M_PI/(leeway*associated.box.w))*(playerx-ileeway*associated.box.w))) { 
            // Trata como uma função seno para determinar se ativa a colisão
            // leeway é usado para reduzir o tamanho dessa colisao se necessario
            active = true;
            associated.damage = 1;
            // printf("ENTER2'\n");

        }
        else {
            active = false;
            associated.damage = -1;
        }
    }
    else {
        active = false;
        associated.damage = -1;
    }
    // Enquanto funcional --------------------------------------------------------------------------
    speed.y = 0;
 
    associated.box.x += (speed.x) * dt;
    associated.box.y += (speed.y)* dt;
    
    // Atualiza animação de acordo com a movimentação
    Animator *animator = static_cast<Animator *>(associated.GetComponent("Animator"));
    //SpriteRenderer *renderer = static_cast<SpriteRenderer *>(associated.GetComponent("SpriteRenderer"));
    if (animator)
    {
        animator->Update(dt);
        animator->SetAnimation("flowing");         
    }
    SpriteRenderer *rend = (SpriteRenderer *)associated.GetComponent("SpriteRenderer");
    if (side > 0) {
        rend->SetFrame(animator->GetCurrentFrame(),SDL_FLIP_VERTICAL);
    }
    else {
        rend->SetFrame(animator->GetCurrentFrame(),SDL_FLIP_NONE);
    }
}

void BeatWave::Render() {}

bool BeatWave::Is(const std::string &type)
{
    return type == "BeatWave";
}


void BeatWave::NotifyCollision(GameObject &other)
{
}

