#ifndef SPRITE_H
#define SPRITE_H

#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE
#include "SDL_include.h"

#include <string>
#include "Game.h"
#include "Rect_Vec2.h"

class Sprite
{
public:
    // Construtores
    Sprite();
    Sprite(const std::string &file, int frameCountW = 1, int frameCountH = 1);

    // Destruidor
    ~Sprite();

    // Carrega a imagem
    void Open(const std::string &file);

    // Configura o retângulo de clipping
    void SetClip(int x, int y, int w, int h);

    // Renderiza a imagem
    void Render(int x, int y);
    void Render(int x, int y, float angle);
    void Render(int x, int y, int w, int h);
    void Render(int x, int y, int w, int h, float angle); // Nova sobrecarga para permitir redimensionamento

    // Controle de animação
    void SetFrame(int frame);
    void SetFrameCount(int frameCountW, int frameCountH);

    // Getters para número de frames
    int GetFrameCountW() const;
    int GetFrameCountH() const;

    // Retorna a largura e altura de um frame individual
    int GetWidth() const;
    int GetHeight() const;

    // Verifica se a textura está carregada
    bool IsOpen() const;

    // Corrige fundo
    bool cameraFollower;
    void SetCameraFollower(bool follow);

    // escala e flip
    void SetScale(float x, float y);
    Vec2 GetScale() const;
    void SetFlip(SDL_RendererFlip newFlip);
    void SetAlpha(int transp = 255);

private:
    SDL_Texture *texture; // A textura carregada
    int width;            // Largura total da imagem
    int height;           // Altura total da imagem
    SDL_Rect clipRect;    // Retângulo de clipping

    int frameCountW;  // Número de frames na horizontal
    int frameCountH;  // Número de frames na vertical
    int currentFrame; // Frame atual da animação

    Vec2 scale = Vec2(1.0f, 1.0f); // inicializa com escala padrão
    SDL_RendererFlip flip = SDL_FLIP_NONE;
};

#endif // SPRITE_H
