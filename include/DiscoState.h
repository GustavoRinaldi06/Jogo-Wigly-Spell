#pragma once
#include "State.h"
#include "Music.h"
#include "EndState.h"
#include "DanceFloor.h"
#include "Sound.h"
#include "Transition.h"
#include "Timer.h"
#include <vector>
#include <string>

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
    void DontStopTheMusic();

private:
    DanceFloor *dancefloor = nullptr;

    // DJ
    Music backgroundMusic;
    Sound vinilSwipe;
    std::vector<std::string> playlist;
    std::string currentTrack;
    float furthest_chara;
    float base_chara = -600;

    Timer victoryTimer;
    bool bossDefeated = false;

    Transition *transitionEffect = nullptr;
    bool gameOverTriggered = false;
};
