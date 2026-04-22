#ifndef TILESET_H
#define TILESET_H

#include <string>
#include "Sprite.h"

class TileSet
{
    public:
        TileSet(int w, int h, const std::string &file);
        void RenderTile(unsigned index, float x, float y);
        int GetTileWidth();
        int GetTileHeight();

        void SetCameraFollower(bool follow); // Parallax debug
    private:
        Sprite tileSet;
        int tileWidth, tileHeight, tileCount;
};
#endif // TILESET_H