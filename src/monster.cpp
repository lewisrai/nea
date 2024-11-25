#include "monster.hpp"

// Init //

Monster::Monster(const SoundArray &sounds, int level, float multiplier, int xPos, int yPos) {
  if (GetRandomValue(0, 99) < multiplier * 3) {
    // Strong
    m_monsterType = MonsterType::Strong;
    m_rect.width = BLOCK_SIZE * 4 / 3;
    m_rect.height = BLOCK_SIZE * 8 / 3;
    m_textureIDs = {ID_ENEMY_STRONG_LEFT, ID_ENEMY_STRONG_RIGHT};
    multiplier *= 1.5;
  }
  else {
    // Normal
    m_monsterType = MonsterType::Normal;
    m_rect.width = BLOCK_SIZE;
    m_rect.height = BLOCK_SIZE * 2;
    m_textureIDs = {ID_ENEMY_LEFT, ID_ENEMY_RIGHT};
  }

  // Position, Size
  m_rect.x = xPos;
  m_rect.y = yPos;

  // Sound
  m_sounds = {sounds[ID_MONSTER_DAMAGED], sounds[ID_MONSTER_JUMPED], sounds[ID_MONSTER_WALKING]};

  // State
  m_currentTextureID = m_textureIDs[FACING_RIGHT];

  // Stats
  m_maxImmunity = 20;
  m_damage = 4 * multiplier;
  m_health = 30 * multiplier;
  m_knockback = 15 * multiplier;
  m_points = 10 * multiplier;

  // Movement
  m_acceleration = 1.1 + level * 0.04;
  m_jumpVelocity = -16.0 + level * 0.08;
  m_maxSpeed.x = 4.5 + level * 0.15;
}


// Public Methods //

void Monster::Draw(const TextureArray &textures, float cameraLimitLeft, float cameraLimitRight) const {
  //Skip if not near players
  if (m_rect.x < cameraLimitLeft) return;
  else if (m_rect.x > cameraLimitRight) return;

  DrawTexture(textures[m_currentTextureID], m_rect.x, m_rect.y, WHITE);
}


void Monster::UpdateAI(const BlockList &blocks, const PlayerArray &players) {
  // Reset inputs
  m_inputState[JUMP] = false;
  m_inputState[GO_LEFT] = false;
  m_inputState[GO_RIGHT] = false;

  // Find closest player
  const Vector2 centre = {m_rect.x + m_rect.width / 2, m_rect.y + m_rect.height / 2};

  const Vector2 p0Position = players[0].GetCentre();
  const Vector2 p1Position = players[1].GetCentre();

  const float p0Distance = (centre.x - p0Position.x) * (centre.x - p0Position.x) + (centre.y - p0Position.y) * (centre.y - p0Position.y);
  const float p1Distance = (centre.x - p1Position.x) * (centre.x - p1Position.x) + (centre.y - p1Position.y) * (centre.y - p1Position.y);

  const Vector2 closestPlayer = (p0Distance < p1Distance) ? p0Position : p1Position;

  // Update inputs
  if (centre.y > closestPlayer.y) m_inputState[JUMP] = true;

  if (centre.x > closestPlayer.x) {
    m_inputState[GO_LEFT] = true;
    if (IsBlockInFront(blocks, {centre.x - BLOCK_SIZE, centre.y + BLOCK_SIZE / 2})) m_inputState[JUMP] = true;
  }
  else {
    m_inputState[GO_RIGHT] = true;
    if (IsBlockInFront(blocks, {centre.x + BLOCK_SIZE, centre.y + BLOCK_SIZE / 2})) m_inputState[JUMP] = true;
  }
}


void Monster::UpdateReceived(const PlayerArray &players, ProjectileList &projectiles, Wall &wall) {
    // Wall
  if (m_rect.x < wall.GetFrontPositionX()) {
    wall.IncreaseSpeed();
    m_health = 0;
  }

  // Check if player has immunity
  if (m_immunity != 0) return;

  int damage = 0;
  float knockback = 0;

  // Swords
  for (const Player &player : players) {
    // Skip if not near monster
    if (player.GetPositionX() < m_rect.x - MAX_BLOCK_LENGTH) continue;
    else if (player.GetPositionX() > m_rect.x + MAX_BLOCK_LENGTH) continue;

    for (const Weapon &weapon : player.GetWeapons()) {
      damage = weapon.CheckSwordCollision(m_rect);

      if (damage != 0) {
        // Knockback
        if (m_monsterType != MonsterType::Strong) {
          knockback = weapon.GetKnockback();
          m_velocity.y -= knockback / 4;
          if (player.GetFacingLeft()) m_velocity.x -= knockback;
          else m_velocity.x += knockback;
        }

        // Damage
        PlaySound(m_sounds[DAMAGED]);
        m_health -= damage;
        m_immunity = m_maxImmunity;
        return;
      }
    }
  }

  // Guns
  for (Projectile &projectile : projectiles) {
    // Skip if not near monster
    if (projectile.GetPositionX() < m_rect.x - MAX_BLOCK_LENGTH) continue;
    else if (projectile.GetPositionX() > m_rect.x + MAX_BLOCK_LENGTH) continue;

    if (CheckCollisionCircleRec(projectile.GetPosition(), projectile.GetRadius(), m_rect)) {
      // Knockback
      if (m_monsterType != MonsterType::Strong) {
        knockback = projectile.GetKnockback();
        if (projectile.GetFacingLeft()) m_velocity.x -= knockback;
        else m_velocity.x += knockback;
      }

      // Damage
      PlaySound(m_sounds[DAMAGED]);
      m_health -= projectile.GetDamage();
      projectile.SetFalse();
      m_immunity = m_maxImmunity;
      return;
    }
  }
}


// Getters //

float Monster::GetDamageIfCollision(Rectangle rect) const {
  if (CheckCollisionRecs(m_rect, rect)) return m_damage;
  else return 0;
}


float Monster::GetHealth() const {return m_health;}


float Monster::GetKnockback() const {return m_knockback;}


float Monster::GetPoints() const {return m_points;}


// Functions //

bool IsBlockInFront(const BlockList &blocks, Vector2 checkPosition) {
  for (const Block &block : blocks) {
    // Skip if not near block
    if (block.GetPositionX() < checkPosition.x - MAX_BLOCK_LENGTH) continue;
    else if (block.GetPositionX() > checkPosition.x + MAX_BLOCK_LENGTH) break;

    if (CheckCollisionPointRec(checkPosition, block.GetRectangle())) return true;
  }
  return false;
}


void GenerateEnemies(const SoundArray &sounds, const BlockList &blocks, MonsterList &monsters, int level) {
  // Calculate level multiplier
  float multiplier = 1;
  while (level > 1) {
    multiplier *= 1.1;
    level--;
  }

  // Spawn monsters by chance based on monsterThreshold
  int monsterThreshold = multiplier * 5;
  Rectangle blockRect = {0, 0, 0, 0};

  for (const Block &block : blocks) {
    blockRect = block.GetRectangle();

    if (blockRect.width / BLOCK_SIZE < 3) continue;
    else if (blockRect.x < -BORDER_HORIZONTAL + LEVEL_PADDING * 2) continue;

    if (GetRandomValue(0, 99) < monsterThreshold) monsters.push_back(Monster(sounds, level, multiplier, blockRect.x + BLOCK_SIZE, blockRect.y - BLOCK_SIZE * 2));
  }
}


MonsterList RemoveDeadEnemies(MonsterList oldMonsters, float &score) {
  MonsterList newMonsters;

  for (int i = 0; i < (int)oldMonsters.size(); i++) {
    if (oldMonsters[i].GetHealth() > 0) newMonsters.push_back(oldMonsters[i]);
    else score += oldMonsters[i].GetPoints();
  }
  return newMonsters;
}
