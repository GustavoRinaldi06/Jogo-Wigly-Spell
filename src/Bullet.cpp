#include "Bullet.h"
#include "SpriteRenderer.h"
#include "Character.h"
#include "Collider.h"
#include "Animation.h"
#include "Animator.h"
#include "Smoke.h"
#include <cmath>

Bullet::Bullet(GameObject &associated, float angle, float speed, int damage, float maxDistance,  const std::string &spritePath, int color, int bultype)
    : Component(associated), damage(damage), distanceLeft(maxDistance)
{
    
    type = bultype;
    int wcount;
    int hcount;
    Vec2 colscale;
    int row = type % 3;
    auto animator = new Animator(associated);
    purpledurability = 0;
    
    if (type == 0) {
        wcount = 4;
        hcount = 3;
        colscale = Vec2(0.8,0.7);
    }
    else if (type == 1) {
        wcount = 4;
        hcount = 3;
        colscale = Vec2(0.8,0.8);
    }
    else if (type == 2) {
        wcount = 4;
        hcount = 3;
        colscale = Vec2(1,1);
    }
    else if (type == 3) {
        wcount = 4;
        hcount = 2;
        colscale = Vec2(1,1);
        purpledurability = 3;
    }
    auto renderer = new SpriteRenderer(associated, spritePath, wcount, hcount);
    animator->AddAnimation("idle", Animation(4*row, 4*row + 3,(1.0/24)));
    animator->AddAnimation("idle2", Animation(4*(row+1), 4*(row+1) + 3,(1.0/24)));
    animator->SetAnimation("idle");
    associated.AddComponent(animator);
    
    velocity = Vec2(std::cos(angle), std::sin(angle)) * speed;

    
    renderer->SetCameraFollower(false);
    associated.AddComponent(renderer);

    associated.angleDeg = angle * 180 / M_PI + 90; // Adicionar 90 para a bala ficar horizontal em relação a arma

    Collider *col = new Collider(associated,colscale);
    col->tag = "bullet";
    associated.AddComponent(col);
    
    bulletcolor = color;
}

Bullet::~Bullet() {
    
}

void Bullet::Update(float dt)
{
    Vec2 displacement = velocity * dt;
    associated.box.x += displacement.x;
    associated.box.y += displacement.y;
    distanceLeft -= displacement.Magnitude();

    Animator *animator = static_cast<Animator *>(associated.GetComponent("Animator"));
    
    if (animator) {
        animator->Update(dt);
    }
    
    if (distanceLeft <= 0)
    {
        associated.RequestDelete();
    }

    SpriteRenderer *rend = (SpriteRenderer *)associated.GetComponent("SpriteRenderer");
    if (bulletcolor == 3 && purpledurability <= 0) {
        animator->SetAnimation("idle2");
        damage = 12;    // Reduz dano se perdeu as bordas
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
            associated.StandardSmoke();
            associated.RequestDelete();
        }
    }
}
