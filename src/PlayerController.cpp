#include "PlayerController.h"
#include "InputManager.h"
#include "Character.h"
#include "Camera.h"
#include "GameData.h"

PlayerController::PlayerController(GameObject &associated)
    : Component(associated) {}

void PlayerController::Start() {}

void PlayerController::Update(float dt)
{
    InputManager &input = InputManager::GetInstance();
    Character *character = (Character *)associated.GetComponent("Character");
    if (!character)
        return;

    Vec2 direction;

    if (input.IsKeyDown('a'))
        direction.x -= 10;
    if (input.IsKeyDown('d'))
        direction.x += 10;

    if (GameData::gameMode == 0){ // Troca para menu
        if (input.IsKeyDown('w'))
            direction.y -= 10;
        if (input.IsKeyDown('s'))
            direction.y += 10;
    }


    if (direction.Magnitude() > 0.1f)
    {
        Vec2 target = associated.box.GetCenter() + direction.Normalize() * 100;
        character->Issue(Character::Command(Character::CommandType::MOVE, target.x, target.y));
    }

    if (input.MousePress(LEFT_MOUSE_BUTTON)) // Atira a spell padrão
    {
        if (GameData::aimed) // Se aimed for True, mira no mouse
        {
            float mouseX = input.GetMouseX() + Camera::GetInstance().GetPosition().x;
            float mouseY = input.GetMouseY() + Camera::GetInstance().GetPosition().y;
            character->Shoot1(Vec2(mouseX, mouseY));
        }
        else // Se aimed for False, atira reto para a frente
        {
            Vec2 center = associated.box.GetCenter();
            float targetX = center.x + 500.0f;

            character->Shoot1(Vec2(targetX, center.y));
        }
    }

    if (input.KeyPress('e')) // Atira a spell de mistura
    {
        if (GameData::aimed) // Se aimed for True, mira no mouse
        {
            float mouseX = input.GetMouseX() + Camera::GetInstance().GetPosition().x;
            float mouseY = input.GetMouseY() + Camera::GetInstance().GetPosition().y;
            character->UseSpell(Vec2(mouseX, mouseY));
        }
        else // Se aimed for False, atira reto para a frente
        {
            Vec2 center = associated.box.GetCenter();
            float targetX = center.x + 500.0f;

            character->UseSpell(Vec2(targetX, center.y));
        }
    }
}

void PlayerController::Render() {}

bool PlayerController::Is(const std::string &type)
{
    return type == "PlayerController";
}
