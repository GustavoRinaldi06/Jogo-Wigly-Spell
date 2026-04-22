#include "../include/Resources.h"
#include "../include/Game.h"

// Definição das variáveis estáticas da classe Resources
std::unordered_map<std::string, SDL_Texture *> Resources::imageTable;
std::unordered_map<std::string, Mix_Music *> Resources::musicTable;
std::unordered_map<std::string, Mix_Chunk *> Resources::soundTable;
std::unordered_map<std::string, TTF_Font *> Resources::fontTable;

// ================== TEXTURAS ==================

SDL_Texture *Resources::GetImage(const std::string &file)
{
    auto it = imageTable.find(file);
    if (it != imageTable.end()) // Caso encontre
    {
        return it->second; // Já carregada
    }

    SDL_Texture *texture = IMG_LoadTexture(Game::GetInstance().GetRenderer(), file.c_str());
    if (!texture)
    {
        SDL_Log("GetImage - Erro ao carregar imagem '%s': %s", file.c_str(), SDL_GetError());
        return nullptr;
    }

    imageTable[file] = texture;
    return texture;
}

void Resources::ClearImages()
{
    for (auto &pair : imageTable)
    {
        SDL_DestroyTexture(pair.second);
    }
    imageTable.clear();
}

// ================== MÚSICAS ==================

Mix_Music *Resources::GetMusic(const std::string &file)
{
    auto it = musicTable.find(file);
    if (it != musicTable.end())
    {
        return it->second;
    }

    Mix_Music *music = Mix_LoadMUS(file.c_str());
    if (!music)
    {
        SDL_Log("GetMusic - Erro ao carregar música '%s': %s", file.c_str(), Mix_GetError());
        return nullptr;
    }

    musicTable[file] = music;
    return music;
}

void Resources::ClearMusics()
{
    for (auto &pair : musicTable)
    {
        Mix_FreeMusic(pair.second);
    }
    musicTable.clear();
}

// ================== SONS ==================

Mix_Chunk *Resources::GetSound(const std::string &file)
{
    auto it = soundTable.find(file);
    if (it != soundTable.end())
    {
        return it->second;
    }

    Mix_Chunk *chunk = Mix_LoadWAV(file.c_str());
    if (!chunk)
    {
        SDL_Log("GetSound - Erro ao carregar som '%s': %s", file.c_str(), Mix_GetError());
        return nullptr;
    }

    soundTable[file] = chunk;
    return chunk;
}

void Resources::ClearSounds()
{
    for (auto &pair : soundTable)
    {
        Mix_FreeChunk(pair.second);
    }
    soundTable.clear();
}

// ================== FONTES ==================

TTF_Font *Resources::GetFont(const std::string &file, int fontSize)
{
    std::string key = file + "#" + std::to_string(fontSize);
    auto it = fontTable.find(key);
    if (it != fontTable.end())
        return it->second;

    TTF_Font *font = TTF_OpenFont(file.c_str(), fontSize);
    if (!font)
    {
        SDL_Log("GetFont - Erro ao carregar fonte: '%s'  (%s)", file.c_str(), TTF_GetError());
        return nullptr;
    }
    fontTable[key] = font;
    return font;
}

void Resources::ClearFonts()
{
    for (auto &pair : fontTable)
    {
        TTF_CloseFont(pair.second);
    }
    fontTable.clear();
}