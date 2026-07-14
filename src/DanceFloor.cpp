#include "DanceFloor.h"
#include "GameData.h"
#include "Game.h"
#include "State.h"
#include "SpriteRenderer.h"
#include "GameData.h"
#include <cstdlib>

DanceFloor::DanceFloor(GameObject &associated, std::string spritePath, float tileSize)
    : Component(associated), tileSize(tileSize), spritePath(spritePath), tilesCreated(false)
{
    grid.resize(rows, std::vector<int>(cols, 0));
    tileObjects.resize(rows, std::vector<GameObject *>(cols, nullptr));
    associated.layer = 2.5;
    GameData::danceFloorPtr = this;
}

DanceFloor::~DanceFloor()
{
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            if (tileObjects[r][c] != nullptr)
            {
                tileObjects[r][c]->RequestDelete();
            }
        }
    }
}

void DanceFloor::Update(float dt)
{
    auto &state = Game::GetInstance().GetCurrentState();

    // Cria as tiles se não existirem
    if (!tilesCreated)
    {
        float spacing = 40.0f;
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                GameObject *tileGO = new GameObject();
                tileGO->box.x = associated.box.x + (c * tileSize) + (c * spacing);
                tileGO->box.y = associated.box.y + (r * tileSize) + (r * spacing);
                tileGO->box.w = tileSize;
                tileGO->box.h = tileSize;
                tileGO->layer = associated.layer;

                SpriteRenderer *renderer = new SpriteRenderer(*tileGO, spritePath, 2, 3);
                renderer->SetCameraFollower(false);
                tileGO->AddComponent(renderer);
                renderer->SetFrame(0, SDL_FLIP_NONE);

                tileObjects[r][c] = tileGO;
                state.AddObject(tileGO);
            }
        }
        tilesCreated = true;
        return;
    }

    // Se NÃO estiver no modo de dança, apaga a luz da pista
    if (GameData::gameMode != 0)
    {
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                grid[r][c] = 5;
            }
        }
    }

    // Aplica os frames dos Sprites baseados estritamente na matriz grid
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            if (tileObjects[r][c] != nullptr)
            {
                SpriteRenderer *renderer = (SpriteRenderer *)tileObjects[r][c]->GetComponent("SpriteRenderer");
                if (renderer)
                {
                    renderer->SetFrame(grid[r][c], SDL_FLIP_NONE);
                }
            }
        }
    }
}

void DanceFloor::ActivateDisco()
{
    int baserow = 0;
    int basecol = 0;
    
    for (int k = 0;k < 4;k++){
        int pinkpick = rand() % 6;
        int yellowpick = rand() % 6;
        while (yellowpick == pinkpick) {
            yellowpick = rand() % 6;
        }
        int bluepick = rand() % 6;
        while (bluepick == pinkpick || bluepick == yellowpick) {
            bluepick = rand() % 6;
        }

        int greenpick = rand() % 6;
        while (greenpick == pinkpick || greenpick == yellowpick || greenpick == bluepick) {
            greenpick = rand() % 6;
        }
        int picklist[4] = {pinkpick,yellowpick,bluepick,greenpick};
        
        for (int i = 0; i < 6; i++) {
            int fixedcol = -1;
            for (int j = 0; j < 4;j++) {
                if (i == picklist[j]) {
                    fixedcol = j;
                    break;
                }
            }

            if (fixedcol < 0) {
                grid[baserow+int(i/3)][basecol+(i%3)] = rand() % 4;
            }
            else {
                grid[baserow+int(i/3)][basecol+(i%3)] = fixedcol;
            }
            
            
        }
        basecol +=3;
        baserow += 2*int(basecol/6);
        basecol = basecol % 6;
    }
    /*
    for (int r = baserow; r < 3 + baserow; r++)
    {
        for (int c = basecol; c < 2 + basecol; c++)
        {
            grid[r][c] = 0 + (rand() % 4);
        }
    }
    */
}

void DanceFloor::Error()
{
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            grid[r][c] = 4;
        }
    }
}

void DanceFloor::Render() {}

int DanceFloor::GetColorAtPosition(float x, float y)
{
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            GameObject *tile = tileObjects[r][c];
            if (tile != nullptr)
            {
                // Verifica se as coordenadas estão dentro desta tile
                if (x >= tile->box.x && x <= (tile->box.x + tile->box.w) && y >= tile->box.y && y <= (tile->box.y + tile->box.h))
                {
                    return grid[r][c];
                }
            }
        }
    }
    return -1; // Caso ele esteja nos espaços vazios
}

bool DanceFloor::Is(const std::string &type)
{
    return type == "DanceFloor";
}
