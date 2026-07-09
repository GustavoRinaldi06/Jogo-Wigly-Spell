#include "Potion.h"
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

Potion::Potion(GameObject &associated, const std::string &spritePath,int num)
    : Component(associated)
{
    auto renderer = new SpriteRenderer(associated, spritePath, 10, 1);
    if (num == 2) {
        renderer->SetFrame(0,SDL_FLIP_NONE);
    }
    else {
        renderer->SetFrame(8,SDL_FLIP_NONE);
    }
    
    associated.AddComponent(renderer);
    number=num;
}

Potion::~Potion()
{
}

void Potion::Start()
{}


void Potion::Update(float dt)
{
    
    SpriteRenderer *renderer = (SpriteRenderer *)associated.GetComponent("SpriteRenderer");
    int color1 = 0;
    int color2 = 0;
    bool half = false;
    try {
        // Ordem das cores está trocada na sprite
         color2 =  3 - static_cast<int>((Character::player->GetColorInventory()).at(1));
    }
    catch (...) {
        color2 = 0;
    }
    try {
         color1 =  3 - static_cast<int>((Character::player->GetColorInventory()).at(0));
    }
    catch (...) {
        color1 = 0;
    }
    if (number == 2) {
        int mixcolor;
        if (color1 == 0) {
            mixcolor = color2;
            half = true;
        }
        else if (color2 == 0) {
            mixcolor = color1;
            half = true;
        }
        else if (color1 != color2) {
            mixcolor = 3;
        }
        else {
            mixcolor = color1;
        }
        
        if (mixcolor == 3) {
            renderer->SetFrame(5,SDL_FLIP_NONE); 
        }
        else if (mixcolor == 2) {
            if (half) {
                renderer->SetFrame(2,SDL_FLIP_NONE); 
            }
            else {
                renderer->SetFrame(4,SDL_FLIP_NONE); 
            }
            
        }
        else {
            if (mixcolor == 0) {
                renderer->SetFrame(0,SDL_FLIP_NONE); 
            }
            else if (half) {
                renderer->SetFrame(1,SDL_FLIP_NONE); 
            }
            else {
                renderer->SetFrame(3,SDL_FLIP_NONE); 
            }
        }
        
    }
    else if (number == 1) {
        int sum = 0;
        if (color2 == 1) {
            sum = -1;
        }
        else if (color2 == 2) {
            sum = 1;
        }
        renderer->SetFrame(8+sum,SDL_FLIP_NONE);
    }
    else if (number == 0) {
        int sum = 0;
        if (color1 == 1) {
            sum = -1;
        }
        else if (color1 == 2) {
            sum = 1;
        }
        renderer->SetFrame(8+sum,SDL_FLIP_NONE);
    }
    renderer->SetCameraFollower(1.0);
}

void Potion::Render() {}

bool Potion::Is(const std::string &type)
{
    return type == "Potion";
}
