#ifndef HEADER_PLAYER
#define HEADER_PLAYER

#include <array>
#include <raylib.h>
#include <vector>
#include "entity.hpp"
#include "monster.hpp"
#include "prototype.hpp"
#include "resource.hpp"
#include "weapon.hpp"

#define PLAYER_ACCELERATION 1.6       // Base
#define PLAYER_JUMP_VELOCITY -20      // Base
#define PLAYER_MAX_SPEED_HORIZONTAL 9 // Base

#define PLAYER_IMMUNITY 30             // Base
#define PLAYER_MAX_APART 1000000       // Base


// Player Define //

class Player : public Entity {
  public:
    // Init
    Player(const SoundArray &sounds, bool player0, int keyUp, int keyLeft, int keyRight, int keyAttack0, int keyAttack1);

    // Method - draw to screen
    void Draw(const TextureArray &textures) const;
    // Method - updates weapons + stops user from switching weapons while one is in use
    void UpdateAttack(float deltaTime, ProjectileList &projectiles);
    // Method - checks for user inputs
    void UpdateInput();
    // Method - check if monsters collide with the player
    void UpdateReceived(const BlockList &blocks, const MonsterList &monsters, const Wall &wall, float &playersHealth);

    // Getter
    Rectangle GetRectangle() const;
    // Getter
    Vector2 GetCentre() const;
    // Getter
    WeaponList GetWeapons() const;

    // Setter - sets player attributes
    void ApplyUpgrades(const SoundArray &sounds, const std::array<int, 10> &upgrades);
    // Setter - sets position to start of the level and gives player extra immunity
    void Reset();

  private:
    WeaponList m_weapons; // Weapons

    const bool m_player0; // State

    float m_damageReduction = 1; // Stat

    std::array<int, 5> m_keybinds; // Input
};


// Tether Define //

class Tether {
  public:
    // Method - draw to screen
    void Draw() const;
    // Method - check if players are too far from each other + set colour and thickness of line
    void Update(const PlayerArray &players, float &playersHealth);

  private:
    Vector2 m_startPosition = {0, 0}; // Position
    Vector2 m_endPosition = {0, 0};   // Position

    Color m_colour = {0, 0, 0, 0}; // State
    float m_thickness = 0;         // State
};


#endif
