#ifndef HEADER_MONSTER
#define HEADER_MONSTER

#include <raylib.h>
#include <string>
#include <vector>
#include "entity.hpp"
#include "level.hpp"
#include "player.hpp"
#include "prototype.hpp"
#include "resource.hpp"
#include "weapon.hpp"


// Define //

enum class MonsterType {
  Normal,
  Strong
};


class Monster : public Entity {
  public:
    // Init
    Monster(const SoundArray &sounds, int level, float multiplier, int xPos, int yPos);

    // Method - checks if in view of camera before drawing to screen
    void Draw(const TextureArray &textures, float cameraLimitLeft, float cameraLimitRight) const;
    // Method - looks for closest player and attempts to reach them
    void UpdateAI(const BlockList &blocks, const PlayerArray &players);
    // Method - check if a weapon or projectile collides with the monster
    void UpdateReceived(const PlayerArray &players, ProjectileList &projectiles, Wall &wall);

    // Getter
    float GetDamageIfCollision(Rectangle rect) const;
    // Getter
    float GetHealth() const;
    // Getter
    float GetKnockback() const;
    // Getter
    float GetPoints() const;

  private:
    MonsterType m_monsterType; // State

    float m_damage;    // Stat
    float m_health;    // Stat
    float m_knockback; // Stat
    float m_points;    // Stat
};


// Functions //

// Checks if point collisides with a block, allows monsters to jump up hills
bool IsBlockInFront(const BlockList &blocks, Vector2 checkPosition);

// Randomly spawns monsters on top of blocks - difficulty increases as level increases
void GenerateEnemies(const SoundArray &sounds, const BlockList &blocks, MonsterList &enemies, int level);

// Removes enemies below 0 health from list and adds points to score
MonsterList RemoveDeadEnemies(MonsterList oldEnemies, float &score);


#endif
