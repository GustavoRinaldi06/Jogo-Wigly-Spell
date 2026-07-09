#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <memory>
#include "Component.h"
#include "Rect_Vec2.h"

class GameObject
{
public:
    GameObject(); // Inicializa isDead como falso
    ~GameObject();

    void Update(float dt);
    void Render();
    bool IsDead() const; // Retorna se está morto
    void RequestDelete(); // Atribui True para IsDead

    void AddComponent(Component *cpt); // adiciona componentes a objeto
    void RemoveComponent(Component *cpt); // remove componentes a objeto
    Component *GetComponent(const std::string &type); // retorna ponteiro de componente

    Rect box;

    bool started;
    void Start();
    float layer = 5.0;

    double angleDeg = 0; // usado para rotação

    void NotifyCollision(GameObject &other);
    void StandardSmoke();
    int color = 0;
    int blockable = -1;
    int damage = -1;

private:
    std::vector<Component *> components; // Um objeto é composto de componentes
    bool isDead;
};

#endif // GAMEOBJECT_H
