#include "Bullet.h"
#include "SpriteRenderer.h"
#include "Character.h"
#include "Collider.h"
#include <cmath>

Bullet::Bullet(GameObject &associated, float angle, float speed, int damage, float maxDistance, bool targetsPlayer, const std::string &spritePath, int color)
    : Component(associated), damage(damage), distanceLeft(maxDistance), targetsPlayer(targetsPlayer)
{
    velocity = Vec2(std::cos(angle), std::sin(angle)) * speed;
    int wcount = 1;
    purpledurability = 0;
    if (color == 3) {
        wcount = 2;
        purpledurability = 3;
    }
    auto renderer = new SpriteRenderer(associated, spritePath, wcount, 1);
    renderer->SetCameraFollower(false);
    associated.AddComponent(renderer);

    associated.angleDeg = angle * 180 / M_PI + 90; // Adicionar 90 para a bala ficar horizontal em relação a arma

    Collider *col = new Collider(associated,Vec2(1,1));
    col->tag = "bullet";
    associated.AddComponent(col);

    bulletcolor = color;
}

void Bullet::Update(float dt)
{
    Vec2 displacement = velocity * dt;
    associated.box.x += displacement.x;
    associated.box.y += displacement.y;
    distanceLeft -= displacement.Magnitude();



    if (distanceLeft <= 0)
    {
        associated.RequestDelete();
    }

    SpriteRenderer *rend = (SpriteRenderer *)associated.GetComponent("SpriteRenderer");
    if (bulletcolor == 3 && purpledurability <= 0) {
        rend->SetFrame(1,SDL_FLIP_NONE);
        damage = 12;    // Reduz dano se perdeu as bordas
    }
    else if (bulletcolor == 3){
        rend->SetFrame(0,SDL_FLIP_NONE);
        
    }
    
}

void Bullet::Render()
{
}

bool Bullet::Is(const std::string &type)
{
    return type == "Bullet";
}

int Bullet::GetDamage() const
{
    return damage;
}

void Bullet::NotifyCollision(GameObject &other)
{
    Collider *collider = (Collider *)other.GetComponent("Collider");
    if (!other.GetComponent("Bullet") && !associated.IsDead() && collider)
    {
        if (collider->tag == "solid" && bulletcolor == 0){
            associated.RequestDelete();
        }
    }
}
