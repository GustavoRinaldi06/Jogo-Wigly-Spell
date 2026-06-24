#pragma once
#include "State.h"
#include "Music.h"
#include "EndState.h"
#include "DanceFloor.h"

class DiscoState : public State
{
public:
    DiscoState();
    ~DiscoState();

    void LoadAssets() override;
    void Update(float dt) override;
    void Render() override;

    void Start() override;
    void Pause() override;
    void Resume() override;


    Text *hpText = nullptr;
    Text *spellText = nullptr;
    Text *bosshpText = nullptr;

    DanceFloor* getDancefloor();

private:
    DanceFloor *dancefloor = nullptr;
    Music backgroundMusic;
};
