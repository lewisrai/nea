#include "player.hpp"

// Player Init //

Player::Player(const SoundArray &sounds, bool player0, int keyUp, int keyLeft, int keyRight, int keyAttack0, int keyAttack1) : m_player0(player0) {
  const int textureOffset = (m_player0) ? 0 : 2;
  // Size
  m_rect.width = BLOCK_SIZE;
  m_rect.height =  BLOCK_SIZE * 2;

  // Sounds
  m_sounds = {sounds[ID_PLAYER_DAMAGED], sounds[ID_PLAYER_JUMPED], sounds[ID_PLAYER_WALKING]};

  // States
  m_keybinds = {keyUp, keyLeft, keyRight, keyAttack0, keyAttack1};
  m_skipCollisionBlock = ID_BLOCK_AIR;
  m_textureIDs = {ID_PLAYER1_LEFT + textureOffset, ID_PLAYER1_LEFT + 1 + textureOffset};
  m_currentTextureID = m_textureIDs[FACING_RIGHT];
  m_maxImmunity = PLAYER_IMMUNITY;

  // Movement
  m_acceleration = PLAYER_ACCELERATION;
  m_jumpVelocity = PLAYER_JUMP_VELOCITY;
  m_maxSpeed.x = PLAYER_MAX_SPEED_HORIZONTAL;

  // Starter weapons
  m_weapons.push_back(Weapon(sounds, WeaponType::Sword));
}


// Public Methods //

void Player::Draw(const TextureArray &textures) const {
  // Player
  DrawTexture(textures[m_currentTextureID], m_rect.x, m_rect.y, WHITE);

  // Weapons
  for (const Weapon &weapon : m_weapons) weapon.Draw(textures);
}


void Player::UpdateAttack(float deltaTime, ProjectileList &projectiles) {
  if (m_weapons.size() == 1) {
    // Player has 1 weapon
    m_weapons[0].SetActive(true);
    m_weapons[0].UpdateWeapon(m_facingLeft, deltaTime, m_keybinds[3], m_rect, projectiles);
  }
  else {
    // Player has 2 weapons
    if (IsKeyDown(m_keybinds[ATTACK_0]) && m_weapons[1].GetIdle()) {
      m_weapons[0].SetActive(true);
      m_weapons[1].SetActive(false);
    }
    else if (IsKeyDown(m_keybinds[ATTACK_1]) && m_weapons[0].GetIdle()) {
      m_weapons[0].SetActive(false);
      m_weapons[1].SetActive(true);
    }

    m_weapons[0].UpdateWeapon(m_facingLeft, deltaTime, m_keybinds[3], m_rect, projectiles);
    m_weapons[1].UpdateWeapon(m_facingLeft, deltaTime, m_keybinds[4], m_rect, projectiles);
  }
}


void Player::UpdateInput() {for (int i = 0; i < 5; i++) m_inputState[i] = IsKeyDown(m_keybinds[i]);}


void Player::UpdateReceived(const BlockList &blocks, const MonsterList &monsters, const Wall &wall, float &playersHealth) {
  // Magma Blocks
  for (const Block &block : blocks) {
    if (block.GetPositionX() < m_rect.x - MAX_BLOCK_LENGTH) continue;
    else if (block.GetTextureID() != ID_BLOCK_MAGMA) continue;
    else if (block.GetPositionX() > m_rect.x + MAX_BLOCK_LENGTH) break;

    // Check collision 1 "pixel" below centre bottom of player
    if (CheckCollisionPointRec({m_rect.x + m_rect.width, m_rect.y + m_rect.height + 1}, block.GetRectangle())) {
      PlaySound(m_sounds[DAMAGED]);
      playersHealth -= block.GetDamage();
      m_immunity = m_maxImmunity;
      return;
    }
  }

  // Wall
  if (m_rect.x < wall.GetFrontPositionX()) playersHealth = 0;

  // Check if player has immunity
  if (m_immunity != 0) return;

  int damage = 0;
  float knockback = 0;

  // Monsters
  for (const Monster &monster : monsters) {
    // Skip if not near monster
    if (monster.GetPositionX() < m_rect.x - MAX_BLOCK_LENGTH) continue;
    else if (monster.GetPositionX() > m_rect.x + MAX_BLOCK_LENGTH) continue;

    damage = monster.GetDamageIfCollision(m_rect);
    if (damage != 0) {
      // Knockback
      knockback = monster.GetKnockback();
      m_velocity.y -= knockback / 4;
      if (monster.GetFacingLeft()) m_velocity.x -= knockback;
      else m_velocity.x += knockback;

      // Damage
      PlaySound(m_sounds[DAMAGED]);
      playersHealth -= damage;
      m_immunity = m_maxImmunity;
      return;
    }
  }
}


// Getters //

Rectangle Player::GetRectangle() const {return m_rect;}


Vector2 Player::GetCentre() const {return {m_rect.x + m_rect.width / 2, m_rect.y + m_rect.height / 2};}


WeaponList Player::GetWeapons() const {return m_weapons;}


// Setters //

void Player::ApplyUpgrades(const SoundArray &sounds, const std::array<int, 10> &upgrades) {
  // Player upgrades
  const int offset = (m_player0) ? 0 : 5;
  const int movement = upgrades[2 + offset];
  const int immunity = upgrades[3 + offset];
  int damageReduction = upgrades[4 + offset];

  m_acceleration = PLAYER_ACCELERATION + movement * 0.05;
  m_jumpVelocity = PLAYER_JUMP_VELOCITY + movement * 0.1;

  m_maxImmunity = PLAYER_IMMUNITY + immunity * 2.5;

  m_damageReduction = 1;
  while (damageReduction > 0) {
    m_damageReduction *= 0.95;
    damageReduction--;
  }

  // Weapon upgrades
  if (m_weapons.size() == 1) {
    m_weapons[0].SetTier(upgrades[0 + offset]);

    if (upgrades[1 + offset] > -1) {
      m_weapons.push_back(Weapon(sounds, WeaponType::Gun));
      m_weapons[1].SetTier(upgrades[1 + offset]);
    }
  }
  else {
    m_weapons[0].SetTier(upgrades[0 + offset]);
    m_weapons[1].SetTier(upgrades[1 + offset]);
  }
}


void Player::Reset() {
  m_rect.x = -BORDER_HORIZONTAL + LEVEL_PADDING + BLOCK_SIZE;
  m_rect.y = -(BLOCK_SIZE * 3);
  m_immunity = 180;
}


// Tether Public Methods //

void Tether::Draw() const {DrawLineEx(m_startPosition, m_endPosition, m_thickness, m_colour);}


void Tether::Update(const PlayerArray &players, float &playersHealth) {
  m_startPosition = players[0].GetCentre();
  m_endPosition = players[1].GetCentre();

  const float distanceApart = (m_startPosition.x - m_endPosition.x) * (m_startPosition.x - m_endPosition.x) + (m_startPosition.y - m_endPosition.y) * (m_startPosition.y - m_endPosition.y);

  // Check if players are too far from each other
  if (distanceApart > PLAYER_MAX_APART) {
    playersHealth = 0;
    return;
  }

  // Update colour and thickness depending on distance apart
  const float ratio = 1 - (distanceApart / PLAYER_MAX_APART);
  m_colour = {(unsigned char)(255 - (ratio * 255)), (unsigned char)(ratio * 255), 0, 128};
  m_thickness = 0.25 * BLOCK_SIZE * ratio;
}
