#ifndef LIGHT_H
#define LIGHT_H

#include "Component.h"
#include <string>

class GameObject;
class SpriteRenderer;

class Light : public Component
{
public:
    Light(GameObject &associated, const std::string &spritePath);
    ~Light();

    void Start() override;
    void Update(float dt) override;
    void Render() override;
    bool Is(const std::string &type) override;

private:
    SpriteRenderer *spriteRenderer;
};

#endif