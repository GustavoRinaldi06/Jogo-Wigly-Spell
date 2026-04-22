#include "../include/TileMap.h"
#include "../include/Game.h"
#include "../include/GameObject.h"
#include "../include/Camera.h"

#include <fstream>
#include <sstream> 
#include <iostream>

    TileMap::TileMap(GameObject &associated, const std::string &file, TileSet *tileSet):Component(associated), mapWidth(0), mapHeight(0), mapDepth(0)
{
    Load(file);
}

void TileMap::Load(const std::string &file)
{
    std::ifstream mapFile(file);
    char comma; // pular a vírgula

    if (mapFile.is_open())
    {
        // Lê as dimensões do mapa
        mapFile >> mapWidth >> comma >> mapHeight >> comma >> mapDepth >> comma;

        // Verifica se as dimensões foram lidas corretamente (DEBUG)
        if (mapFile.fail())
        {
            std::cerr << "Erro ao ler as dimensões do mapa no arquivo: " << file << std::endl;
            return;
        }

        // Cria a matriz de tiles para o mapa
        tileMatrix.resize(mapWidth * mapHeight * mapDepth);

      
        // le os tiles para preencher a matriz
        for (int z = 0; z < mapDepth; ++z)
        {
            for (int y = 0; y < mapHeight; ++y)
            {
                for (int x = 0; x < mapWidth; ++x)
                {
                    int index;
                    char c; // pular a vírgula
                    mapFile >> index >> c;

                    // DEBUG
                    if (mapFile.fail()){
                        std::cerr << "Erro ao ler o índice do tile em " << x << ", " << y << ", " << z << "\n";
                        return;
                    }

                    tileMatrix[(z * mapHeight + y) * mapWidth + x] = index;
                }
            }
        }
        mapFile.close();
    }
    else
    {
        std::cerr << "Erro ao abrir o arquivo do mapa: " << file << std::endl;
    }
}

void TileMap::SetTileSet(TileSet *tileSet)
{
    this->tileSet.reset(tileSet);
}

int &TileMap::At(int x, int y, int z)
{
    int index = x + (y * mapWidth) + (z * mapWidth * mapHeight);
    return tileMatrix[index]; // Retorna o tile na posição (x, y, z)
}

void TileMap::Render()
{
    for (int i = 0; i < mapDepth; ++i)
    {
        float parallaxFactor = 0.8f + (i * 0.2f); //  fundo se move mais devagar
        RenderLayer(i, parallaxFactor);
    }
}

void TileMap::RenderLayer(int layer, float parallaxFactor)
{
    int tileW = tileSet->GetTileWidth();
    int tileH = tileSet->GetTileHeight();

    // Garante que a câmera será aplicada manualmente
    tileSet->SetCameraFollower(false);

    // Calcula deslocamento da câmera com parallax
    Vec2 cam = Camera::GetInstance().GetPosition() * parallaxFactor;

    for (int y = 0; y < mapHeight; ++y)
    {
        for (int x = 0; x < mapWidth; ++x)
        {
            int index = At(x, y, layer);
            if (index != -1)
            {
                //  posição do tile
                float worldX = associated.box.x + x * tileW;
                float worldY = associated.box.y + y * tileH;

                // adicona o parallax e a camera
                float posX = worldX + cam.x * (1.0f - parallaxFactor);
                float posY = worldY + cam.y * (1.0f - parallaxFactor);

                tileSet->RenderTile(index, posX, posY);
            }
        }
    }
}

int TileMap::GetWidth() const
{
    return mapWidth;
}

int TileMap::GetHeight() const
{
    return mapHeight;
}

int TileMap::GetDepth() const
{
    return mapDepth;
}


// Importadas de Component
void TileMap::Update(float dt)
{
    // TileMap é estático, então talvez nada precise ser feito aqui
}

bool TileMap::Is(const std::string &type)
{
    return type == "TileMap";
}