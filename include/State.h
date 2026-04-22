#ifndef STATE_H
#define STATE_H

#include <vector>
#include <memory>
#include "GameObject.h"

class State
{
public:

    State();
    virtual ~State();

    // Métodos virtuais 
    virtual void LoadAssets() = 0;
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;

    virtual void Start() = 0;
    virtual void Pause() = 0;
    virtual void Resume() = 0;

    // Interface para gerenciar objetos
    virtual std::weak_ptr<GameObject> AddObject(GameObject *object);
    virtual std::weak_ptr<GameObject> GetObjectPtr(GameObject *object);

    // Comunicação com a engine
    bool PopRequested() const; // Muda estado
    bool QuitRequested() const;

protected:
    virtual void StartArray();
    virtual void UpdateArray(float dt);
    virtual void RenderArray();

    // Flags de controle 
    bool popRequested;
    bool quitRequested;
    bool started;

    // Lista de objetos pertencentes ao estado
    std::vector<std::shared_ptr<GameObject>> objectArray;
};

#endif // STATE_H
