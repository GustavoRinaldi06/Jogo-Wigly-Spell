#include "Sound.h"
#include "../include/Resources.h"

Sound::Sound() : chunk(nullptr), channel(-1) {}

Sound::Sound(const std::string &file) : chunk(nullptr), channel(-1)
{
    Open(file);
}

Sound::~Sound()
{
    if (chunk != nullptr)
    {
        Stop();
        chunk = nullptr;
    }
}

// Função para tocar o som
void Sound::Play(int times)
{
    if (chunk != nullptr)
    {
        // times - 1 porque o número de repetições é loops - 1
        channel = Mix_PlayChannel(-1, chunk, times - 1);
    }
}

// Função para parar o som
void Sound::Stop()
{
    if (channel != -1)
    {
        Mix_HaltChannel(channel);
        channel = -1;
    }
}

// Função para abrir o arquivo de som
void Sound::Open(const std::string &file)
{
    chunk = Resources::GetSound(file);
    if (chunk == nullptr)
    {
        printf("Erro ao carregar o som: %s\n", Mix_GetError());
    }
}

// Função para verificar se o som foi aberto corretamente
bool Sound::IsOpen() const
{
    return chunk != nullptr;
}