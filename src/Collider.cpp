#include "Collider.h"
#include "GameObject.h"
#include "Camera.h"
#include "Game.h"
#include <cmath>

#define INCLUDE_SDL
//#define DEBUG
#include "SDL_include.h"

Collider::Collider(GameObject &associated, Vec2 scale, Vec2 offset)
    : Component(associated), tag("") , scale(scale), offset(offset) {}

void Collider::Update(float)
{
    Rect &baseBox = associated.box;
    box.w = baseBox.w * scale.x;
    box.h = baseBox.h * scale.y;
    box.SetCenter(baseBox.GetCenter() + offset.Rotate(associated.angleDeg * M_PI / 180));
}

void Collider::Render()
{
#ifdef DEBUG // Cria caixa de colisão para teste
    Vec2 cam = Camera::GetInstance().GetPosition();
    SDL_Rect dstRect = {(int)(box.x - cam.x), (int)(box.y - cam.y), (int)box.w, (int)box.h};
    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(Game::GetInstance().GetRenderer(), &dstRect);
#endif
}

bool Collider::Is(const std::string &type)
{
    return type == "Collider";
}

int Collider::ColDir(Collider *other)
{
    Vec2 centerdist = other->box.GetCenter() - box.GetCenter();
    Vec2 overlaplimit =  Vec2((other->box.w + box.w),(other->box.h + box.h));
    Vec2 distance = overlaplimit - Vec2(abs(centerdist.x),abs(centerdist.y));
    if (distance.x < 0) {
        distance.x = 9999;
    }
    if (distance.y < 0) {
        distance.y = 9999;
    }
    distance = Vec2(distance.x/overlaplimit.x,distance.y/overlaplimit.y);
    if (distance.y <= distance.x) {
        if (centerdist.y >= 0) {
            return 0;   // Baixo
        }
        else {
            return 1; // Cima
        }
    }
    else {
        if (centerdist.x >= 0) {
            return 2;   // direita
        }
        else {
            return 3; // esquerda
        }
    }
}

void Collider::SetScale(Vec2 scale) { this->scale = scale; }
void Collider::SetOffset(Vec2 offset) { this->offset = offset; }
Vec2 Collider::GetScale() {return this->scale;}
Vec2 Collider::GetOffset() {return this->offset;}