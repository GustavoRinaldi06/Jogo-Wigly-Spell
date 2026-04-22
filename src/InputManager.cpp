#include "../include/InputManager.h"

// Inicializa os valores essenciais
InputManager::InputManager()
{
    quitRequested = false;
    updateCounter = 0;
    mouseX = 0;
    mouseY = 0;

    for (int i = 0; i < 6; ++i)
    {
        mouseState[i] = false;
        mouseUpdate[i] = 0;
    }
}

InputManager::~InputManager() {} // Não implementado ainda
// Retorna a instancia estática
InputManager &InputManager::GetInstance()
{
    static InputManager instance;
    return instance;
}

void InputManager::Update()
{
    SDL_Event event;

    // Avança o contador de frames
    updateCounter++;

    // Atualiza posição do mouse
    SDL_GetMouseState(&mouseX, &mouseY);

    // Reseta quit
    quitRequested = false;

    // Loop de eventos
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT: // Clique no X, Alt+F4, etc.
            quitRequested = true;
            break;

        case SDL_MOUSEBUTTONDOWN: // Pressionamento de botão do mouse
            if (event.button.button < 6) // SDL numera os 5 botões do mouse
            {
                mouseState[event.button.button] = true;
                mouseUpdate[event.button.button] = updateCounter;
            }
            break;

        case SDL_MOUSEBUTTONUP: // Botão do mouse foi solto
            if (event.button.button < 6) // SDL numera os 5 botões do mouse
            {
                mouseState[event.button.button] = false;
                mouseUpdate[event.button.button] = updateCounter;
            }
            break;

        case SDL_KEYDOWN: // Pressionamento de tecla
            if (!event.key.repeat)
            {
                keyState[event.key.keysym.sym] = true;
                keyUpdate[event.key.keysym.sym] = updateCounter;
            }
            break;

        case SDL_KEYUP: // Uma tecla foi solta
            keyState[event.key.keysym.sym] = false;
            keyUpdate[event.key.keysym.sym] = updateCounter;
            break;
        }
    }
}

bool InputManager::KeyPress(int key)
{
    return keyUpdate[key] == updateCounter && keyState[key];
}

bool InputManager::KeyRelease(int key)
{
    return keyUpdate[key] == updateCounter && !keyState[key];
}

bool InputManager::IsKeyDown(int key)
{
    return keyState[key];
}

bool InputManager::MousePress(int button)
{
    return mouseUpdate[button] == updateCounter && mouseState[button];
}

bool InputManager::MouseRelease(int button)
{
    return mouseUpdate[button] == updateCounter && !mouseState[button];
}

bool InputManager::IsMouseDown(int button)
{
    return mouseState[button];
}

int InputManager::GetMouseX()
{
    return mouseX;
}

int InputManager::GetMouseY()
{
    return mouseY;
}

bool InputManager::QuitRequested()
{
    return quitRequested;
}
