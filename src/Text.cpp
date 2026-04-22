#include "Text.h"
#include "Game.h"
#include "Resources.h"
#include "Camera.h"

Text::Text(GameObject &associated, const std::string &fontFile, int fontSize, TextStyle style, const std::string &text, SDL_Color color)
    : Component(associated), arqFonte(fontFile), tamFonte(fontSize), estilo(style), cor(color), texto(text), texture(nullptr)
{
    // Cria a textura inicial
    RemakeTexture();
}

Text::~Text()
{
    if (texture)
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

void Text::Update(float dt)
{}

void Text::Render()
{
    if (!texture)
        return;

    // Renderizar de acordo com a camera, por mais que não seja usado nesse trabalho
    //Vec2 camPos = Camera::GetInstance().GetPosition();
    Vec2 renderPos(associated.box.x, associated.box.y);
    if (!cameraFollower)
    {
        renderPos = renderPos - Camera::GetInstance().GetPosition();
    }

    SDL_Rect dstRect;
    dstRect.x = static_cast<int>(renderPos.x);
    dstRect.y = static_cast<int>(renderPos.y);
    dstRect.w = clipRect.w;
    dstRect.h = clipRect.h;

    SDL_RenderCopyEx(Game::GetInstance().GetRenderer(), texture, &clipRect, &dstRect, associated. angleDeg, nullptr, SDL_FLIP_NONE);
}

// Set de texto: Atualizar valor e mandar refazer
void Text::SetText(const std::string &newText)
{
    texto = newText;
    RemakeTexture();
}

void Text::SetColor(SDL_Color newColor)
{
    cor = newColor;
    RemakeTexture();
}

void Text::SetStyle(TextStyle newStyle)
{
    estilo = newStyle;
    RemakeTexture();
}

void Text::SetFontSize(int newFontSize)
{
    tamFonte = newFontSize;
    RemakeTexture();
}

void Text::RemakeTexture()
{
    if (texture) // Dstroi textura atual
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

// Cria nova caixa de text
    TTF_Font *font = Resources::GetFont(arqFonte, tamFonte);
    if (!font) //  Não achou font
    {
        return;
    }

    SDL_Surface *surface = nullptr;
    switch (estilo)
    {
    case SOLID:
        surface = TTF_RenderText_Solid(font, texto.c_str(), cor);
        break;
    case SHADED:
    {
        SDL_Color bg = {0, 0, 0, 255}; // Fundo preto
        surface = TTF_RenderText_Shaded(font, texto.c_str(), cor, bg);
        break;
    }
    case BLENDED:
        surface = TTF_RenderText_Blended(font, texto.c_str(), cor);
        break;
    }

    if (!surface) // Não conseguiu criar
    {
        return;
    }

    // Cria uma textura a partir da surface
    SDL_Renderer *renderer = Game::GetInstance().GetRenderer();
    texture = SDL_CreateTextureFromSurface(renderer, surface);

    clipRect.x = 0;
    clipRect.y = 0;
    clipRect.w = surface->w;
    clipRect.h = surface->h;

    associated.box.w = surface->w;
    associated.box.h = surface->h;

    // libera surface da CPU
    SDL_FreeSurface(surface);
}

bool Text::Is(const std::string &type)
{
    return (type == "Text");
}

void Text::SetCameraFollower(bool follow)
{
    cameraFollower = follow;
}
