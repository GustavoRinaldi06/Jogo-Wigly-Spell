#ifndef CHARACTER_H
#define CHARACTER_H

#include "Component.h"
#include "Rect_Vec2.h"
#include "Timer.h"
#include "Sound.h"
#include "Bullet.h"
#include <queue>
#include <memory>

class Character : public Component
{
public:
    enum class CommandType
    {
        MOVE
    };

    struct Command
    {
        CommandType type;
        Vec2 pos;

        Command(CommandType type, float x, float y) : type(type), pos(x, y) {}
    };

    Character(GameObject &associated, const std::string &spritePath);
    ~Character();

    void Start() override;
    void Update(float dt) override;
    void Render() override;
    bool Is(const std::string &type) override;
    void Issue(Command task);

    static Character *player; // ponteiro global (caso seja o player)

    // Colisão
    void NotifyCollision(GameObject &other) override;
    GameObject *GetGameObject() { return &associated; }

    // Acessar informações privadas
    int GetHP() const;
    int GetCool() const;

    // Atirar ao mesmo tempo que anda
    void Shoot1(Vec2 targetPos);
    void Shoot2(Vec2 targetPos);

private:
    std::queue<Command> taskQueue;

    // Valores de controle plataforma
    Vec2 speed;
    float gravity = 1000.0f; // Gravidade
    bool isOnGround = false;
    bool jumped = false;
    bool Djumped = false;

    // Config basica
    float linearSpeed;
    int hp;
    

    // Death e hit
    Sound hitSound, fallSound, deathSound;
    Timer damageCooldown;

    Timer deathTimer;
    bool deathAnimTriggered{false};

    // Walk config
    Sound walkSound;
    Timer walkSoundTimer; // Timer para controlar o som de andar
    void walkSoundCall(float dt);

    // Speel basica
    Timer spellTimer; //cooldown
    Sound spellSound;

    // Speel 2
    Timer spell2Timer; //cooldown
    Sound spell2Sound;
    Sound noSpell;

    // Controle de sprite
    int facingDir = 1; // Direção X

    // Verificadores de inversão
    bool wasInverted;
    bool Inversion;
};

#endif
