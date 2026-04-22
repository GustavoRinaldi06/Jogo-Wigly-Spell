#include "../include/TileSet.h"
#include <iostream>

TileSet::TileSet(int w, int h, const std::string &file)
    : tileSet(file), tileWidth(w), tileHeight(h), tileCount(0)
{

    if (!tileSet.IsOpen())
    {
        std::cerr << "Erro: não foi possível carregar o tileset: " << file << "\n";
        return;
    }

    int texWidth = tileSet.GetWidth();   
    int texHeight = tileSet.GetHeight(); 

    int columns = texWidth / tileWidth;
    int rows = texHeight / tileHeight;
    tileCount = columns * rows;

    // Define a nova grade de frames para corresponder aos tiles
    tileSet.SetFrameCount(columns, rows);
}

void TileSet::RenderTile(unsigned index, float x, float y)
{
    if (index < (unsigned)tileCount)
    {
        tileSet.SetFrame(index);
        tileSet.Render((int)x, (int)y);
    }
    else
    {
        std::cerr << "Erro: índice inválido (" << index << ")" << "\n";
    }
}

// sets
int TileSet::GetTileWidth()
{
    return tileWidth;
}

int TileSet::GetTileHeight()
{
    return tileHeight;
}

void TileSet::SetCameraFollower(bool follow) //Parallax debug
{
    tileSet.SetCameraFollower(follow);
}
