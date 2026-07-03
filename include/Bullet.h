#ifndef BULLET_H
#define BULLET_H

#include "Component.h"
#include "Rect_Vec2.h"

class Bullet : public Component
{
public:
    Bullet(GameObject &associated, float angle, float speed, int damage, float maxDistance, const std::string &spritePath, int color = 0, int type = 0);

    void Update(float dt) override;
    void Render() override;
    bool Is(const std::string &type) override;

    int GetDamage() const;
    void NotifyCollision(GameObject &other) override;
    int damage;
    int bulletcolor = 0;
    int purpledurability = 3;
    int type;
private:
    Vec2 velocity;
    float distanceLeft;
};

#endif
