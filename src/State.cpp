#include "../include/State.h"

// Construtor
State::State() : popRequested(false), quitRequested(false), started(false) {}

State::~State()
{
    objectArray.clear();
}

// Adiciona um objeto ao vetor e retorna
std::weak_ptr<GameObject> State::AddObject(GameObject *goRaw)
{
    std::shared_ptr<GameObject> go(goRaw);
    objectArray.emplace_back(go);

    if (started)
    {
        go->Start();
    }

    return std::weak_ptr<GameObject>(go);
}

std::weak_ptr<GameObject> State::GetObjectPtr(GameObject *goRaw)
{
    for (auto &obj : objectArray)
    {
        if (obj.get() == goRaw)
        {
            return std::weak_ptr<GameObject>(obj);
        }
    }
    return std::weak_ptr<GameObject>(); // vazio
}

// Flags de controle
bool State::PopRequested() const
{
    return popRequested; // Muda estado
}

bool State::QuitRequested() const
{
    return quitRequested; // Feha o jogo
}

// Inicializa todos os objetos do estado
void State::StartArray()
{
    size_t n = objectArray.size();
    for (size_t i = 0; i < n; ++i)
        objectArray[i]->Start();
}

// Atualiza todos os objetos do estado
void State::UpdateArray(float dt)
{
    for (size_t i = 0; i < objectArray.size(); ++i)
    {
        objectArray[i]->Update(dt);
    }
}


// Renderiza todos os objetos do estado
void State::RenderArray()
{
    for (auto &obj : objectArray)
    {
        obj->Render();
    }
}
