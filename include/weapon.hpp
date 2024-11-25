#ifndef HEADER_WEAPON
#define HEADER_WEAPON

#include <cmath>
#include <raylib.h>
#include "level.hpp"
#include "resource.hpp"

#define GUN_DAMAGE 6        // Base
#define GUN_KNOCKBACK 12    // Base
#define GUN_BULLET_SPEED 20 // Base
#define GUN_RELOAD_SPEED 6  // Base

#define SWORD_SWING_START 1 // Base
#define SWORD_SWING_END 130 // Base
#define SWORD_SWING_IDLE 65 // Base
#define SWORD_DAMAGE 17     // Base
#define SWORD_KNOCKBACK 12  // Base
#define SWORD_SWING_SPEED 4 // Base


// Projectile Define //

class Projectile {
  public:
    // Init
    Projectile(float radius, Vector2 position, float damage, float knockback, float speed);

    // Method - checks if in view of camera before drawing to screen
    void Draw(float cameraLimitLeft, float cameraLimitRight) const;
    // Method - check for collisions with blocks
    void UpdateCollision(const BlockList &blocks);
    // Method - update position by bullet speed and deltaTime
    void UpdateMovement(float deltaTime);

    // Getter
    bool GetActive() const;
    // Getter
    bool GetFacingLeft() const;
    // Getter
    float GetDamage() const;
    // Getter
    float GetKnockback() const;
    // Getter
    float GetPositionX() const;
    // Getter
    float GetRadius() const;
    // Getter
    Vector2 GetPosition() const;

    // Setter
    void SetFalse();

  private:
    const float m_radius; // Size
    Vector2 m_position;   // Position

    bool m_active = true; // State

    const float m_damage;    // Stat
    const float m_knockback; // Stat
    const float m_speed;     // Stat
};


typedef std::vector<Projectile> ProjectileList;


// Projectile Functions //

// Removes projectiles that have collided with blocks or monsters
ProjectileList RemoveFalseProjectiles(ProjectileList oldProjectiles);


// Weapon Define //

enum class WeaponType {
  Gun,
  Sword
};


class Weapon {
  public:
    // Init
    Weapon(const SoundArray &sounds, WeaponType weaponType);
    // Init
    void Gun(const SoundArray &sounds);
    // Init
    void Sword(const SoundArray &sounds);

    // Method - check if sword collides with rectangle argument
    float CheckSwordCollision(Rectangle rect) const;
    // Method - draw to screen
    void Draw(const TextureArray &textures) const;
    // Method - checks weapon type and calls the update function for that weapon type
    void UpdateWeapon(bool facingLeft, float deltaTime, int keyAttack, Rectangle rect, ProjectileList &projectiles);

    // Getter
    bool GetIdle() const;
    // Getter
    float GetDamage() const;
    // Getter
    float GetKnockback() const;

    // Setter
    void SetActive(bool active);
    // Setter - set stats of weapon
    void SetTier(int tier);

  private:
    // Update Gun
    void UpdateGun(bool facingLeft, float deltaTime, bool keyAttackDown, Rectangle rect, ProjectileList &projectiles);
    // Update Sword
    void UpdateSword(bool facingLeft, float deltaTime, bool keyAttackDown, Rectangle rect);

    Rectangle m_rect = {0, 0, 0, 0}; // Position, size
    Vector2 m_origin = {0, 0};       // Offset

    Sound m_soundAttack; // Sound

    const WeaponType m_weaponType;          // State
    bool m_active = false;                  // State
    bool m_idle = true;                     // State
    float m_rotation = 0;                   // State
    int m_textureID;                        // State
    Rectangle m_textureSize = {0, 0, 0, 0}; // State

    float m_damage;          // Stat - all
    float m_knockback;       // Stat - all
    float m_swingSpeed = 0;  // Stat - swords
    float m_bulletSpeed = 0; // Stat - bullets
    float m_cooldown = 100;  // Stat - bullets
    float m_reloadSpeed = 0; // Stat - bullets
    int m_bulletSize = 0;    // Stat - bullets
};


typedef std::vector<Weapon> WeaponList;


#endif
