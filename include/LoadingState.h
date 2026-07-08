#ifndef LOADING_STATE_H
#define LOADING_STATE_H

#include "State.h"
#include "Text.h"
#include "Animator.h"
#include "Timer.h"
#include <vector>
#include <string>
#include <memory>

enum ResourceType
{
    TYPE_IMAGE,
    TYPE_MUSIC,
    TYPE_SOUND,
    TYPE_FONT
};

struct ResourceItem
{
    std::string path;
    ResourceType type;
    int fontSize;
};

class LoadingState : public State
{
public:
    LoadingState(State *nextState, std::vector<ResourceItem> resourcesToLoad, bool cleanResources = true);
    ~LoadingState();

    void LoadAssets() override;
    void Update(float dt) override;
    void Render() override;

    void Start() override;
    void Pause() override;
    void Resume() override;

private:
    std::unique_ptr<State> nextStatePtr;
    std::vector<ResourceItem> assetsList;

    size_t currentAssetIndex;
    bool clearedPrevious;
    bool shouldClean;

    Timer minimumDisplayTimer;

    Text *percentageText;
    Text *guessingText;

    GameObject *caldeiraoGO;
    GameObject *textGO;
    GameObject *guessingTextGO;
};

#endif