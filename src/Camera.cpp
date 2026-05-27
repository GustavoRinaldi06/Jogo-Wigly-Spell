#include "../include/Camera.h"
#include "../include/InputManager.h"

Camera::Camera() : pos(0, 0), speed(0, 0), focus(nullptr) {}

Camera &Camera::GetInstance()
{
    static Camera instance;
    return instance;
}

void Camera::Follow(GameObject *newFocus)
{
    focus = newFocus;
}

void Camera::Unfollow()
{
    focus = nullptr;
}

void Camera::Update(float dt)
{
    if (focus != nullptr)
    {
        if (focus != nullptr) {
            pos = focus->box.GetCenter() - Vec2(600, 450); // Centralizar a camera em relação ao personagem
        }
        
        // Define os limites da visão (ajustar dependendo da fase)
        float limiteCima = -0.0f;  
        float limiteBaixo = 150.0f; 

        // Aplica o limite na posição Y da câmera
        if (pos.y < limiteCima)
        {
            pos.y = limiteCima; // Bateu no teto, trava a câmera
        }
        else if (pos.y > limiteBaixo)
        {
            pos.y = limiteBaixo; // Bateu no chão, trava a câmera
        }
    }
    else // DEBUG: remover depois para o jogo
    {
        InputManager &input = InputManager::GetInstance();
        speed = Vec2();
        if (focus != nullptr) {
            if (input.IsKeyDown(LEFT_ARROW_KEY) || input.IsKeyDown('a'))
                speed.x = -300;
            if (input.IsKeyDown(RIGHT_ARROW_KEY) || input.IsKeyDown('d'))
                speed.x = 300;
            if (input.IsKeyDown(UP_ARROW_KEY) || input.IsKeyDown('w'))
                speed.y = -300;
            if (input.IsKeyDown(DOWN_ARROW_KEY) || input.IsKeyDown('s'))
                speed.y = 300;

            pos = pos + speed * dt;
        }
        
    }
}

Vec2 Camera::GetPosition() const
{
    return pos;
}

void Camera::SetPosition(Vec2 newPos)
{
    pos = newPos;
}