#include "Bullet.h"
#include "SpriteRenderer.h"
#include "Character.h"
#include "Collider.h"
#include <cmath>

Bullet::Bullet(GameObject &associated, float angle, float speed, int damage, float maxDistance, bool targetsPlayer, const std::string &spritePath)
    : Component(associated), distanceLeft(maxDistance), damage(damage), targetsPlayer(targetsPlayer)
{
    velocity = Vec2(std::cos(angle), std::sin(angle)) * speed;

    auto renderer = new SpriteRenderer(associated, spritePath, 1, 1);
    renderer->SetCameraFollower(false);
    associated.AddComponent(renderer);

    associated.angleDeg = angle * 180 / M_PI + 90; // Adicionar 90 para a bala ficar horizontal em relação a arma

    associated.AddComponent(new Collider(associated));
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
    if (!other.GetComponent("Bullet") && !associated.IsDead())
    {
        Component *comp = other.GetComponent("Character");
        Character *character = (Character *)comp;
        if (character)
        {
            if ((targetsPlayer && character == Character::player) || (!targetsPlayer && character != Character::player)){
                associated.RequestDelete();
                return;
            }
        }
        else
        {
            associated.RequestDelete();
            return;
        }
    }
}
