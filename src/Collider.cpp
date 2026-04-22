#include "Collider.h"
#include "GameObject.h"
#include "Camera.h"
#include "Game.h"
#include <cmath>

#define INCLUDE_SDL
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

void Collider::SetScale(Vec2 scale) { this->scale = scale; }
void Collider::SetOffset(Vec2 offset) { this->offset = offset; }
