#ifndef CHARACTER_H
#define CHARACTER_H

#include "Component.h"
#include "Rect_Vec2.h"
#include "Timer.h"
#include "Sound.h"
#include "Bullet.h"
#include "Bubble.h"
#include "BubbleCD.h"
#include <queue>
#include <memory>


class Character : public Component
{
public:
    // Movimentação
    enum class CommandType
    {
        MOVE
    };

    // Estoque de cores
    enum Color
    {
        NONE,
        RED,
        BLUE
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
    static Rect PlayerBox();

    static Character *player; // ponteiro global (caso seja o player)

    // Colisão
    void NotifyCollision(GameObject &other) override;
    GameObject *GetGameObject() { return &associated; }

    // Acessar informações privadas
    int GetHP() const;
    int GetCool() const;
    int shield = 0;
    Timer shieldTimer;
    bool transparent = false;


    // Atirar ao mesmo tempo que anda
    void Shoot1(Vec2 targetPos);

    // Coleta de cores e disparo de misturas
    bool CollectColor(Color newColor);
    void UseSpell(Vec2 targetPos);
    void ShootMix(Vec2 targetPos, float speed, int damage, float maxDistance, std::string spritePath, int color = 0);
    std::vector<Color> GetColorInventory() const; // Colocar na interface
    bool IsDashing();

    void SetSpeedY(float speedY);
    void ApplyDamage(int damage);
    Timer purpleTimer;
    Timer damageCooldown;

private:
    std::queue<Command> taskQueue;

    // Valores de controle plataforma
    Vec2 speed;
    Vec2 surfacespeed = Vec2(0,0);
    Timer surfaceTimer;
    Timer invTimer;
    float gravity = 1500.0f; // Gravidade
    bool isOnGround = false;
    bool jumped = false;
    bool Djumped = false;
    bool jumping = false;

    bool dashed = false;
    bool dashing = false;

    bool longdash = false;
    Timer dashTimer;
    Timer jumpTimer;
    
    Timer initTimer;

    // Config basica
    float linearSpeed;
    int hp;
    

    // Death e hit
    Sound hitSound, fallSound, deathSound, jumpSound, DjumpSound, dashSound;
    

    Timer deathTimer;
    bool deathAnimTriggered{false};

    // Walk config
    Sound walkSound, wallWalkSound;
    Timer walkSoundTimer; // Timer para controlar o som de andar
    void walkSoundCall(float dt);

    // Speel basica
    Timer spellTimer; //cooldown

    Sound spellSound;

    // Speel de mistura
    std::vector<Color> colorInventory; // Guarda as cores coletadas num vetor
    const size_t MAX_COLORS = 2;
    Timer spellMixTimer; //cooldown
    Sound spell_red_Sound, spell_scarlet_Sound, spell_blue_Sound, spell_darkBlue_Sound, spell_purple_Sound;
    Sound noSpell;

    // Controle de sprite
    int facingDir = 1; // Direção X

    // Verificadores de inversão
    bool wasInverted;
    bool Inversion;
    GameObject *bubble;
    GameObject *bubbleCD;
    GameObject *cauldron;

    // auxilio para troca de modos
    int wasGameMode;
};

#endif
