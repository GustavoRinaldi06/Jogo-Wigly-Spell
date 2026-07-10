#include "State.h"
#include "Music.h"
#include "EndState.h"
#include "Transition.h"
#include "Timer.h"
#include "Sound.h"

class LabState : public State
{
public:
    LabState();
    ~LabState();

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
    bool OverTriggered, tutorialEnd;
    Transition *transitionEffect;
    Timer endTimer;
    Sound SkipSound;
    Sound tutorialSound;

    //tutorial
    SpriteRenderer *tutorialRenderer;
    GameObject *tutorialGameObject;
    int currentTutorialFrame;
};
