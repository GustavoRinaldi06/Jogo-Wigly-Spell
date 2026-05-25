#include "BounceBall.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "State.h" 
#include "Camera.h"
#include "Collider.h"
#include "Game.h"
#include "GameData.h"

#include <iostream>

BounceBall::BounceBall(GameObject &associated, const std::string &spritePath)
    : Component(associated), linearSpeed(200)
{

    auto renderer = new SpriteRenderer(associated, spritePath, 2, 4);
    associated.AddComponent(renderer);

    associated.box.w = 20;  // ou a largura desejada
    associated.box.h = 20; // altura desejada

    // Novos sons
    //hitSound = Sound();
    //deathSound = Sound();
    //fallSound = Sound();

    // Cria as animações
    auto animator = new Animator(associated);
    animator->AddAnimation("floating", Animation(0, 1, 0.5f));
    animator->AddAnimation("bouncing", Animation(2, 3, 0.2f));
    animator->AddAnimation("above", Animation(4, 5, 0.5f));
    animator->AddAnimation("popping", Animation(6, 7, 0.25f));
    associated.AddComponent(animator);

    associated.AddComponent(new Collider(associated,Vec2(0.8,0.8)));
    deathTimer.Restart();
    lifespan.Restart();
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
    if (lifespan.Get() > 10.0f) {
        destroyed = true;
    }

    // Enquanto funcional --------------------------------------------------------------------------

    // Aplica a gravidade --------------------------------------------------------------------
    if (GameData::gameMode == 1)
    {
        // Aplica a gravidade --------------------------------------------------------------------
        if (GameData::inverted == false)
            speed.y += gravity * dt;
        else
            speed.y -= gravity * dt; // gravidade invertida, puxa pro teto
    }
    else
        speed.y = 0;

    associated.box.y += speed.y * dt;
    associated.box.x += speed.x * dt;
    
    // Atualiza animação de acordo com a movimentação
    Animator *animator = static_cast<Animator *>(associated.GetComponent("Animator"));
    if (animator)
    {
        animator->Update(dt);
        if (GameData::gameMode == 0)
            animator->SetAnimation("above");
        else if (bounceTimer.Get() < 0.55f) {
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
                    speed.y = -basespeedy;
                }
                else {
                    speed.y = basespeedy;
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
    if (collider && collider->tag == "ground")
    {
        // Ajusta posição
        associated.box.y = other.box.y - associated.box.h;
        bouncing = true;
        bounceTimer.Restart();
    }

    if (collider && collider->tag == "inverted_ground")
    {
        // Ajusta posição
        associated.box.y = other.box.y + other.box.h;
        bouncing = true;
        bounceTimer.Restart();
        
    }

    // Se colidir com parede
    if (collider && collider->tag == "wall")
    {
        // Destroi se colidir com parede
        associated.RequestDelete();
    }
}

