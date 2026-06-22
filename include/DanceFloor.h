#pragma once
#include "Component.h"
#include "GameObject.h"
#include <vector>
#include <string>

class DanceFloor : public Component
{
public:
    DanceFloor(GameObject &associated, std::string spritePath, float tileSize);
    virtual ~DanceFloor();

    void Update(float dt) override;
    void Render() override;
    bool Is(const std::string &type) override;

    void ActivateDisco();
    int GetColorAtPosition(float x, float y);

private:
    // para gerenciar tamanho da pista
    const int rows = 4;
    const int cols = 9;
    float tileSize;

    std::string spritePath;
    bool tilesCreated;

    std::vector<std::vector<int>> grid;
    std::vector<std::vector<GameObject *>> tileObjects; // referências dos blocos independentes
};