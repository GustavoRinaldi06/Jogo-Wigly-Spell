#include "State.h"
#include "Music.h"
#include "DiscoState.h"
#include "Timer.h"
#include "Transition.h"

class CorridorGhost;

class HallwayState : public State
{
public:
    HallwayState();
    ~HallwayState();

    void LoadAssets() override;
    void Update(float dt) override;
    void Render() override;

    void Start() override;
    void Pause() override;
    void Resume() override;


    Text *hpText = nullptr;
    Text *spellText = nullptr;
    Text *bosshpText = nullptr;



private:

    Music backgroundMusic;
    CorridorGhost *bossComponent;

    Timer victoryTimer;
    bool bossDefeated = false;

    Transition* transitionEffect = nullptr;
    bool gameOverTriggered = false;
};
