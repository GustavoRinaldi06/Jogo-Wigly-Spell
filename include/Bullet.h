#ifndef BULLET_H
#define BULLET_H

#include "Component.h"
#include "Rect_Vec2.h"

class Bullet : public Component
{
public:
    Bullet(GameObject &associated, float angle, float speed, int damage, float maxDistance, bool targetsPlayer, const std::string &spritePath);

    void Update(float dt) override;
    void Render() override;
    bool Is(const std::string &type) override;

    int GetDamage() const;
    void NotifyCollision(GameObject &other) override;
    bool IsTargetingPlayer() const { return targetsPlayer; }
private:
    Vec2 velocity;
    float distanceLeft;
    int damage;
    bool targetsPlayer;
};

#endif
