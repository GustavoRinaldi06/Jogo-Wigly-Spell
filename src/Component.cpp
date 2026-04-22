#include "Component.h"
#include <iostream>

Component::Component(GameObject &associated) : associated(associated) {} // Construtor base

void Component::NotifyCollision(GameObject &other)
{
    //
}