#include "BounceBall.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "State.h" 
#include "Camera.h"
#include "Collider.h"
#include "Game.h"
#include "GameData.h"
#include "StageState.h"

#include <iostream>

BounceBall::BounceBall(GameObject &associated, const std::string &spritePath)
    : Component(associated)
{
    associated.layer = 5.1;
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
    bounceTimer.Set(3.0);
}

BounceBall::~BounceBall()
{
}

void BounceBall::Start()
{}


void BounceBall::Update(float dt)
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

    // Aplica a gravidade --------------------------------------------------------------------
    if (GameData::gameMode == 1 && !bouncing)
    {
        // Aplica a gravidade --------------------------------------------------------------------
        if (GameData::inverted == false)
            speed.y += gravity * dt;
        else
            speed.y -= gravity * dt; // gravidade invertida, puxa pro teto
    }
    else {
        speed.y = 0;
    }
        
    Vec2 uspeed = GameData::universalspeed;
    associated.box.x += (speed.x + uspeed.x) * dt;
    associated.box.y += (speed.y + uspeed.y)* dt;
    
    // Atualiza animação de acordo com a movimentação
    Animator *animator = static_cast<Animator *>(associated.GetComponent("Animator"));
    if (animator)
    {
        if (bouncing) {
            bounceTimer.Update(dt);
        }
        animator->Update(dt);
        if (GameData::gameMode == 0)
            animator->SetAnimation("above");
        else if (bounceTimer.Get() < 0.29f) {
            //printf("%f %i\n",bounceTimer.Get(),bouncing);
            animator->SetAnimation("bouncing");
            Collider *col = static_cast<Collider *>(associated.GetComponent("Collider"));
            col->SetScale(Vec2(0.8,0.4));
            col->SetOffset(Vec2(0,8));
        }
        else {
            if (bouncing) {
                
                bouncing = false;
                Collider *col = static_cast<Collider *>(associated.GetComponent("Collider"));
                col->SetScale(Vec2(0.8,0.8));
                col->SetOffset(Vec2(0,0));
                if (GameData::inverted) {
                    speed.y = basespeedy;
                }
                else {
                    speed.y = -basespeedy;
                }
            }
            animator->SetAnimation("floating");       
        }     
    }

    // Flip horizontal/vertical de acordo a direção ---------------------------------------------------------------
    SpriteRenderer *renderer = static_cast<SpriteRenderer *>(associated.GetComponent("SpriteRenderer"));

    if (renderer && animator)
    {
        int frame = animator->GetCurrentFrame();

        // Começamos sem nenhum flip
        SDL_RendererFlip flip = SDL_FLIP_NONE;

        // Se a gravidade estiver invertida, adiciona o flip vertical ao estado atual (apenas no plataformer)
        if (GameData::inverted && GameData::gameMode == 1){
            flip = (SDL_RendererFlip)(flip | SDL_FLIP_VERTICAL);
        }

        // Aplica o resultado final (que pode ser nenhum, um, ou ambos os flips)
        renderer->SetFrame(frame, flip);

    }

    if(GameData::gameMode == 0){
        speed.y = 0;
    }
    //Collider *collider = (Collider *)associated.GetComponent("Collider");
    //collider->SetScale(Vec2(1,1));
    //collider->SetOffset(Vec2(0,0)); // Ignorando escala e offset para colisão com paredes
}

void BounceBall::Render() {}

bool BounceBall::Is(const std::string &type)
{
    return type == "BounceBall";
}


void BounceBall::NotifyCollision(GameObject &other)
{

    // Se colidir com chão
    Collider *collider = (Collider *)other.GetComponent("Collider");
    Collider * col = (Collider *)associated.GetComponent("Collider");
    int dir = col->ColDir(collider);
    if (collider && collider->tag == "solid") {
        if (dir == 0)
        {
            // Ajusta posição
            if (!bouncing && !GameData::inverted) {
                bouncing = true;
                bounceTimer.Restart();
                speed.y = 0;
            }
            else if (GameData::inverted) {
                speed.y = 0;
            }
            associated.box.y = other.box.y - associated.box.h;
        }

        else if (dir == 1)
        {
            // Ajusta posição
            if (!bouncing && GameData::inverted) {
                bouncing = true;
                bounceTimer.Restart();
                speed.y = 0;
            }
            else if (!GameData::inverted) {
                speed.y = 0;
            }
            associated.box.y = other.box.y + other.box.h;
            
            
        }
        // Se colidir com parede
        else {
                // Destroi se colidir com parede
                destroyed = true;
            }
    }
}

