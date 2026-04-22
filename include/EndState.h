#ifndef ENDSTATE_H
#define ENDSTATE_H

#include "State.h"
#include "Music.h"
#include "GameData.h"
#include "InputManager.h"
#include "Game.h"
#include "TitleState.h"
#include <memory>

class EndState : public State
{
public:
    EndState();
    ~EndState() override;

    void LoadAssets() override;
    void Update(float dt) override;
    void Render() override;

    void Start() override;
    void Pause() override;
    void Resume() override;

private:
    Music backgroundMusic;
};

#endif // ENDSTATE_H
