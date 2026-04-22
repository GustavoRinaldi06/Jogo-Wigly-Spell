#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

class GameObject;

class Component
{
public:
    Component(GameObject &associated);
    virtual ~Component() = default;

    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
    virtual bool Is(const std::string &type) = 0;
    virtual void Start() {}
    virtual void NotifyCollision(GameObject &other);

protected:
    GameObject &associated;
};

#endif // COMPONENT_H
