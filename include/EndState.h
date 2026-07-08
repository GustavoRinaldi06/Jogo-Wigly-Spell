#ifndef END_STATE_H
#define END_STATE_H

#include "State.h"
#include "GameObject.h"
#include "Text.h"
#include "Music.h"

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
    bool IsButtonClicked(GameObject *buttonGO);

    GameObject *btnRetry;
    GameObject *btnMenu;
    GameObject *btnExit;
};

#endif