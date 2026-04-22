#ifndef TILEMAP_H
#define TILEMAP_H

#include "Component.h"
#include "TileSet.h"
#include <memory>
#include <vector>
#include <string>

class TileMap : public Component
{
public:
    TileMap(GameObject &associated, const std::string &file, TileSet *tileSet);

    // Carrega os dados do mapa
    void Load(const std::string &file);
    // Define o TileSet usado no TileMap
    void SetTileSet(TileSet *tileSet);
    // Acessa um tile específico
    int &At(int x, int y, int z = 0);

    void Render(); // renderiza mapa
    void RenderLayer(int layer, float parallaxFactor); // renderiza camada com parallax

    // gets
    int GetWidth() const;
    int GetHeight() const;
    int GetDepth() const;

    // herdada
    void Update(float dt) override;
    bool Is(const std::string &type) override;

private:
    std::vector<int> tileMatrix;
    std::unique_ptr<TileSet> tileSet;
    int mapWidth;
    int mapHeight;
    int mapDepth;

    int Index(int x, int y, int z) const;
    
};

#endif // TILEMAP_H
