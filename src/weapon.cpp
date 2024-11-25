#include "weapon.hpp"

// Projectile Init //

Projectile::Projectile(float radius, Vector2 position, float damage, float knockback, float speed) :  m_radius(radius), m_damage(damage), m_knockback(knockback), m_speed(speed) {
  m_position = position;
}


// Public Methods //

void Projectile::Draw(float cameraLimitLeft, float cameraLimitRight) const {
  //Skip if not near players
  if (m_position.x < cameraLimitLeft) return;
  else if (m_position.x > cameraLimitRight) return;

  DrawCircleV(m_position, m_radius, BROWN);
}


void Projectile::UpdateCollision(const BlockList &blocks) {
  // Check if it is outside the level
  if (m_position.x < -BORDER_HORIZONTAL) m_active = false;
  else if (m_position.x > BORDER_HORIZONTAL) m_active = false;

  if (!m_active) return;

  // Block Collisions
  for (const Block &block : blocks) {
    //Skip if not near block
    if (block.GetPositionX() < m_position.x - MAX_BLOCK_LENGTH) continue;
    else if (block.GetPositionX() > m_position.x + MAX_BLOCK_LENGTH) continue;

    if (CheckCollisionCircleRec(m_position, m_radius, block.GetRectangle())) {
      m_active = false;
      return;
    }
  }
}


void Projectile::UpdateMovement(float deltaTime) {m_position.x += m_speed * deltaTime;}


// Getters //

bool Projectile::GetActive() const {return m_active;}


bool Projectile::GetFacingLeft() const {return m_speed < 0;}


float Projectile::GetDamage() const {return m_damage;}


float Projectile::GetKnockback() const {return m_knockback;}


float Projectile::GetPositionX() const {return m_position.x;}


float Projectile::GetRadius() const {return m_radius;}


Vector2 Projectile::GetPosition() const {return m_position;}


// Setters //

void Projectile::SetFalse() {m_active = false;}


// Functions //

ProjectileList RemoveFalseProjectiles(ProjectileList oldProjectiles) {
  ProjectileList newProjectiles;

  for (int i = 0; i < (int)oldProjectiles.size(); i++) if (oldProjectiles[i].GetActive()) newProjectiles.push_back(oldProjectiles[i]);
  return newProjectiles;
}


// Weapon Init //

Weapon::Weapon(const SoundArray &sounds, WeaponType weaponType) : m_weaponType(weaponType) {
  switch (m_weaponType) {
    case WeaponType::Sword:
      Sword(sounds);
      break;

    case WeaponType::Gun:
      Gun(sounds);
  }
}


void Weapon::Gun(const SoundArray &sounds) {
  // Stats
  m_damage = GUN_DAMAGE;
  m_knockback = GUN_KNOCKBACK;
  m_bulletSpeed = GUN_BULLET_SPEED;
  m_reloadSpeed = GUN_RELOAD_SPEED;
  m_bulletSize = 5;

  // Sounds
  m_soundAttack = sounds[ID_GUN_SHOOT];

  // Position, Size
  m_rect.width = 54;
  m_rect.height = 18;
  m_textureSize.width = 54;
  m_textureSize.height = 18;

  // Offset
  m_origin.x = 0;
  m_origin.y = m_rect.height / 2;

  // States
  m_textureID = ID_GUN;
}


void Weapon::Sword(const SoundArray &sounds) {
  // Stats
  m_damage = SWORD_DAMAGE;
  m_knockback = SWORD_KNOCKBACK;
  m_swingSpeed = SWORD_SWING_SPEED;

  // Sound
  m_soundAttack = sounds[ID_SWORD_SWING];

  // Size
  m_rect.width = 24;
  m_rect.height = 144;
  m_textureSize.width = 24;
  m_textureSize.height = 144;

  // Offset
  m_origin.x = m_rect.width / 2;
  m_origin.y = m_rect.height + BLOCK_SIZE / 4;

  // States
  m_rotation = SWORD_SWING_IDLE;
  m_textureID = ID_SWORD;
}


// Public Methods //

float Weapon::CheckSwordCollision(Rectangle rect) const {
  if (m_weaponType != WeaponType::Sword) return 0;
  else if (m_idle) return 0;

  const double rotationRadians = m_rotation * PI / 180;
  const Vector2 swordStartPosition = {m_rect.x, m_rect.y};

  // m_origin.y is the height of the sword from the point of rotation

  const Vector2 swordEndPosition = {(float)(m_rect.x + m_origin.y * sin(rotationRadians)), (float)(m_rect.y - m_origin.y * cos(rotationRadians))};

  Vector2 *temp = new Vector2;

  // Check line collision with the sides of the rectangle of monster
  if (CheckCollisionLines(swordStartPosition, swordEndPosition, {rect.x, rect.y}, {rect.x + rect.width, rect.y}, temp)) {
    // Top side
    delete temp;
    return m_damage;
  }
  else if (CheckCollisionLines(swordStartPosition, swordEndPosition, {rect.x, rect.y}, {rect.x, rect.y + rect.height}, temp)) {
    // Left side
    delete temp;
    return m_damage;
  }
  else if (CheckCollisionLines(swordStartPosition, swordEndPosition, {rect.x + rect.width, rect.y}, {rect.x + rect.width, rect.y + rect.height}, temp)) {
    // Right side
    delete temp;
    return m_damage;
  }
  else if (CheckCollisionLines(swordStartPosition, swordEndPosition, {rect.x, rect.y + rect.height}, {rect.x + rect.width, rect.y + rect.height}, temp)) {
    // Bottom side
    delete temp;
    return m_damage;
  }
  else {
    // Doesn't collide
    delete temp;
    return 0;
  }
}


void Weapon::Draw(const TextureArray &textures) const {if (m_active) DrawTexturePro(textures[m_textureID], m_textureSize, m_rect, m_origin, m_rotation, WHITE);}


void Weapon::UpdateWeapon(bool facingLeft, float deltaTime, int keyAttack, Rectangle rect, ProjectileList &projectiles) {
  if (!m_active) return;
  const bool keyAttackDown = IsKeyDown(keyAttack);

  switch (m_weaponType) {
    case WeaponType::Sword:
      UpdateSword(facingLeft, deltaTime, keyAttackDown, rect);
      break;

    case WeaponType::Gun:
      UpdateGun(facingLeft, deltaTime, keyAttackDown, rect, projectiles);
  }
}


// Getters //

bool Weapon::GetIdle() const {return m_idle;}


float Weapon::GetDamage() const {return m_damage;}


float Weapon::GetKnockback() const {return m_knockback;}


// Setters //

void Weapon::SetActive(bool active) {
  if (!active) m_idle = true;
  m_active = active;
}


void Weapon::SetTier(int tier) {
  switch (m_weaponType) {
    case WeaponType::Sword:
      m_damage = SWORD_DAMAGE + tier * 3;
      m_knockback = SWORD_KNOCKBACK + tier * 0.5;
      m_swingSpeed = SWORD_SWING_SPEED + tier * 0.2;
      break;

    case WeaponType::Gun:
      m_damage = GUN_DAMAGE + tier * 0.4;
      m_knockback = GUN_KNOCKBACK + tier * 0.25;
      m_bulletSpeed = GUN_BULLET_SPEED + tier;
      m_reloadSpeed = GUN_RELOAD_SPEED + tier * 0.25;
      break;
  }
}


// Private Methods //

void Weapon::UpdateGun(bool facingLeft, float deltaTime, bool keyAttackDown, Rectangle rect, ProjectileList &projectiles) {
  // Set Position
  m_rect.x = rect.x + (rect.width / 2);
  m_rect.y = rect.y + (rect.height / 2);

  // Update Cooldown
  if (!m_idle) m_cooldown -= m_reloadSpeed * deltaTime;
  else if (keyAttackDown) {
    PlaySound(m_soundAttack);
    projectiles.push_back(Projectile(m_bulletSize, {m_rect.x, m_rect.y}, m_damage, m_knockback, (facingLeft) ? -m_bulletSpeed : m_bulletSpeed));
    m_active = true;
    m_idle = false;
    m_cooldown = 100;
  }

  if (m_cooldown < 0) {
    m_idle = true;
    m_cooldown = 100;
  }

  // Set Direction
  if (facingLeft) m_rotation = 180;
  else m_rotation = 0;
}


void Weapon::UpdateSword(bool facingLeft, float deltaTime, bool keyAttackDown, Rectangle rect) {
  // Set Position
  m_rect.x = rect.x + (rect.width / 2);
  m_rect.y = rect.y + (rect.height * 0.625);

  // Update Rotation
  if (m_rotation < 0) m_rotation *= -1;

  if (!m_idle) m_rotation += m_swingSpeed * deltaTime;
  else if (keyAttackDown) {
    PlaySound(m_soundAttack);
    m_active = true;
    m_idle = false;
    m_rotation = SWORD_SWING_START;
  }

  if (m_rotation > SWORD_SWING_END) {
    if (keyAttackDown) {
      PlaySound(m_soundAttack);
      m_rotation = SWORD_SWING_START;
    }
    else {
      m_idle = true;
      m_rotation = SWORD_SWING_IDLE;
    }
  }

  // Set Direction
  if (facingLeft) m_rotation *= -1;
}
