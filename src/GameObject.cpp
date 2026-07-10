#include "GameObject.h"
#include <iostream>
#include "Smoke.h"


GameObject::GameObject() : started(false), angleDeg(0), isDead(false) {}

GameObject::~GameObject() // Destroi todso componentes de um objeto
{
    for (int i = components.size() - 1; i >= 0; --i)
    {
        delete components[i];
    }
    components.clear();
}
void GameObject::Update(float dt)
{
    for (auto &component : components)
    {
        component->Update(dt);
    }
}

void GameObject::Render()
{
    for (auto &component : components)
    {
        component->Render();
    }
}

bool GameObject::IsDead() const
{
    return isDead;
}

void GameObject::RequestDelete()
{
    isDead = true;
}

void GameObject::StandardSmoke(Vec2 scale)
{
    Vec2 pos = Vec2(this->box.x + this->box.w/2.0-25*scale.x,this->box.y+ this->box.h/2.0-25*scale.y);
    float cosangle = (cos((this->angleDeg-90)*M_PI/180));
    float sinangle = (sin((this->angleDeg-90)*M_PI/180));

    pos.x +=  cosangle*this->box.h/3.0;
    pos.y +=  sinangle*this->box.h/3.0; 
    Smoke::CreateSmoke(pos,scale,0);
}

void GameObject::AddComponent(Component *cpt)
{
    components.emplace_back(cpt);
    if (started)
    {
        cpt->Start(); // Chama Start se já iniciado
    }
}

void GameObject::RemoveComponent(Component *cpt)
{
    for (auto it = components.begin(); it != components.end(); ++it)
    {
        if (*it == cpt) // busca componente e apaga
        {
            components.erase(it);
            break;
        }
    }
}

Component *GameObject::GetComponent(const std::string &type)
{
    for (auto &component : components)
    {
        if (component->Is(type))
        {
            return component;
        }
    }
    return nullptr;
}

void GameObject::Start()
{
    for (auto &component : components)
    {
        component->Start();
    }
    started = true;
}

//Trabalho 6
void GameObject::NotifyCollision(GameObject &other)
{
    for (auto &component : components)
    {
        component->NotifyCollision(other);
    }
}
