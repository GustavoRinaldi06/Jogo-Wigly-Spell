#include "../include/Sprite.h"
#include "../include/Game.h"
#include "../include/Resources.h"
#include "../include/Rect_Vec2.h"
#include "Camera.h" 

// Construtores
Sprite::Sprite() : cameraFollower(false), texture(nullptr), width(0), height(0), frameCountW(1), frameCountH(1), currentFrame(0) {
    scale = Vec2(1.0f, 1.0f);
    flip = SDL_FLIP_NONE;
}

Sprite::Sprite(const std::string &file, int frameCountW, int frameCountH)
    : texture(nullptr), frameCountW(frameCountW), frameCountH(frameCountH), currentFrame(0)
{
    scale = Vec2(1.0f, 1.0f);
    flip = SDL_FLIP_NONE;
    Open(file);
}

// Destruidor
Sprite::~Sprite()
{
    texture = nullptr; // A textura será destruída por Resources
}

// Carrega a imagem da textura
void Sprite::Open(const std::string &file)
{
    texture = Resources::GetImage(file);

    if (texture == nullptr)
    {
        SDL_Log("Unable to load texture: %s", SDL_GetError());
        width = height = 0;
        return;
    }

    // Obtém as dimensões da textura completa
    int fullWidth, fullHeight;
    SDL_QueryTexture(texture, nullptr, nullptr, &fullWidth, &fullHeight);

    // Divide a imagem pelo número de frames para obter o tamanho de um único frame
    width = fullWidth / frameCountW;
    height = fullHeight / frameCountH;

    SetFrame(0); // Inicia no primeiro frame
}

// Configura o retângulo de clipping para um único frame
void Sprite::SetClip(int x, int y, int w, int h)
{
    clipRect.x = x;
    clipRect.y = y;
    clipRect.w = w;
    clipRect.h = h;
}
// Seleciona qua deve ser seguido ou não
void Sprite::SetCameraFollower(bool follow)
{
    cameraFollower = follow;
}

// Renderiza a imagem na posição (x, y)
void Sprite::Render(int x, int y, float angle)
{
    if (texture != nullptr)
    {

        if (!cameraFollower)
        {
            Vec2 cam = Camera::GetInstance().GetPosition();
            x -= static_cast<int>(cam.x);
            y -= static_cast<int>(cam.y);
        }
        SDL_Rect dstRect = {x, y, GetWidth(), GetHeight()};
        SDL_RenderCopyEx(Game::GetInstance().GetRenderer(), texture, &clipRect, &dstRect, angle, nullptr, flip);
    }
}

void Sprite::Render(int x, int y, int w, int h, float angle)
{
    if (texture != nullptr)
    {
        if (!cameraFollower)
        {
            Vec2 cam = Camera::GetInstance().GetPosition();
            x -= static_cast<int>(cam.x);
            y -= static_cast<int>(cam.y);
        }
        SDL_Rect dstRect {x, y, static_cast<int>(w * scale.x), static_cast<int>(h * scale.y)};
        SDL_RenderCopyEx(Game::GetInstance().GetRenderer(), texture, &clipRect, &dstRect, angle, nullptr, flip);
    }
}

// Define qual frame da animação deve ser exibido
void Sprite::SetFrame(int frame)
{
    if (frame >= frameCountW * frameCountH)
        return;

    int row = frame / frameCountW; // Descobre a linha do frame
    int col = frame % frameCountW; // Descobre a coluna do frame

    SetClip(col * width, row * height, width, height);
    currentFrame = frame;
}

// Define a quantidade de frames da sprite sheet
void Sprite::SetFrameCount(int frameCountW, int frameCountH)
{
    this->frameCountW = frameCountW;
    this->frameCountH = frameCountH;

    if (texture)
    {
        int texWidth, texHeight;
        SDL_QueryTexture(texture, nullptr, nullptr, &texWidth, &texHeight);
        width = texWidth / frameCountW;
        height = texHeight / frameCountH;
    }

    SetFrame(0); // Reinicia a animação para o primeiro frame
}
// FramesCounts
int Sprite::GetFrameCountW() const
{
    return frameCountW;
}

int Sprite::GetFrameCountH() const
{
    return frameCountH;
}

// Retorna a largura de um frame
int Sprite::GetWidth() const
{
    return static_cast<int>(width * scale.x);
}

// Retorna a altura de um frame
int Sprite::GetHeight() const
{
    return static_cast<int>(height * scale.y);
}

// Verifica se a imagem foi carregada com sucesso
bool Sprite::IsOpen() const
{
    return texture != nullptr;
}

// Trabalho 5
void Sprite::SetScale(float scaleX, float scaleY)
{
    if (scaleX != 0)
        scale.x = scaleX;
    if (scaleY != 0)
        scale.y = scaleY;
}

Vec2 Sprite::GetScale() const
{
    return scale;
}

void Sprite::SetFlip(SDL_RendererFlip newFlip)
{
    flip = newFlip;
}

// DEBUG ----------------------------------------------------------------------------
void Sprite::Render(int x, int y)
{
    Render(x, y, 0.0f);
}

void Sprite::Render(int x, int y, int w, int h)
{
    Render( x, y, w, h, 0.0f);
}