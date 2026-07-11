#include "Floor.h"
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

Floor::Floor(GameObject &associated, const std::string &spritePath, int frame,SDL_RendererFlip flp, float rotation, int tipo, int id)
    : Component(associated)
{
    type = tipo;
    index= id;
    associated.layer = 3.0;
    SpriteRenderer *renderer;
    if (tipo == 0){
        renderer = new SpriteRenderer(associated, spritePath, 2, 1);
        renderer->SetFrame(frame,flp);
    }
    else{
         renderer = new SpriteRenderer(associated, spritePath, 2, 4);
        if (tipo == 1)
        {
            renderer->SetFrame(frame,flp);
        }
        else if (tipo == 3)
        {
            renderer = new SpriteRenderer(associated, spritePath, 2, 1);
            renderer->SetFrame(frame, flp);
        }
        else {
            renderer->SetFrame(frame,flp);
        }
    }
    renderer->SetCameraFollower(0.0);
    changeTime.Restart();
    renderer->SetScale(1,1.0);
    associated.angleDeg = rotation;
    
    associated.AddComponent(renderer);
    
    // Cria as animações
    if (frame != 1 || tipo == 3)
    {
        Collider *col = new Collider(associated,Vec2(1,2));
        // Ajustando collider para evitar de atravessar
        if (flp == SDL_FLIP_NONE) {
            col->SetOffset(Vec2(0,associated.box.h/2));
        }
        else {
            col->SetOffset(Vec2(0,-associated.box.h/2));
        }
        col->tag = "solid";
        associated.AddComponent(col);
    }

    //associated.AddComponent(new Collider(associated,Vec2(1,1)));

}

Floor::~Floor()
{
}

void Floor::Start()
{}

void Floor::Update(float dt)
{
    if (associated.box.x - Camera::GetInstance().GetPosition().x <= limit)
    {
        associated.RequestDelete();
    }
    Vec2 uspeed = GameData::universalspeed;
    associated.box.x += (speed.x + uspeed.x) * dt;
    associated.box.y += (speed.y + uspeed.y) * dt;

    if (type == 1)
    {
        changeTime.Update(dt);
        if (!GameData::discostart)
        {
            return;
        }

        SpriteRenderer *rend = (SpriteRenderer *)associated.GetComponent("SpriteRenderer");
        if (GameData::inversedisco > 0)
        {
            if (GameData::inversedisco == 1)
            {
                changeTime.Restart();
                float x_target = Character::PlayerBox().GetCenter().x;
                int x_id = (int)(x_target / 128);
                int id2 = -10;
                int id3 = -10;

                if (GameData::tripledisco && GameData::bossHP < GameData::p2health) {
                    id2 = x_id +1;
                    id3 = x_id -1;

                    id2 = std::max(0,std::min(id2,7));
                    id3 = std::max(0,std::min(id3,7));
                }
                
                x_id = std::max(0,std::min(x_id,7));
                

                if (!GameData::inverted)
                {
                    x_id += 7;
                    id2 += 7;
                    id3 += 7;
                }
                if (x_id != index && id2 != index && id3 != index)
                {
                    rend->SetFrame(3);
                    associated.damage = -1;
                }
                else
                {
                    rend->SetFrame(4);
                    associated.damage = 1;
                }
            }
            if (changeTime.Get() > 3.0)
            {
                GameData::inversedisco = 0;
                GameData::tripledisco = false;
            }
        }
        else
        {
            associated.damage = -1;


            if (GameData::discoAttackActive)
            {
                if (GameData::discoError)
                {
                    rend->SetFrame(4);
                }
                else if (GameData::discoBlackout) // Se não há erro mas o blackout está ativo (transição)
                {
                    rend->SetFrame(5); 
                }
                else // Caso contrário, mostra a cor normal do round
                {
                    rend->SetFrame(GameData::targetDiscoColor); // Acende na cor do Boss
                }
            }
            else if ((!GameData::inverted && index < 7) || (GameData::inverted && index >= 7))
            {
                if (changeTime.Get() > 2.0)
                {
                    rend->SetFrame(rand() % 4);
                    changeTime.Restart();
                }
            }
            else
            {
                rend->SetFrame(5);
            }
        }
    }
}

void Floor::Render() {}

bool Floor::Is(const std::string &type)
{
    return type == "Floor";
}
