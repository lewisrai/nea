#include "entity.hpp"

// Public Methods //

void Entity::UpdateAudio(Camera2D camera) {
  float distanceFromCamera = (m_rect.x - camera.target.x) / 1000;
  if (distanceFromCamera < 0) distanceFromCamera *= -1;

  const float volume = 1 / (distanceFromCamera + 1);

  for (int i = 0; i < 3; i++) SetSoundVolume(m_sounds[i], volume);
}


void Entity::UpdateImmunity(float deltaTime) {
  // Reduce immunity duration by deltaTime
  m_immunity -= deltaTime;
  if (m_immunity < 0) m_immunity = 0;
}


void Entity::UpdatePosition(const BlockList &blocks, float deltaTime) {
  Rectangle blockRect = {0, 0, 0, 0};
  Rectangle collisionRect = {0, 0, 0, 0};

  // Vertical Movement
  if (m_inputState[JUMP] && m_onGround) {
    PlaySound(m_sounds[JUMPED]);
    m_velocity.y = m_jumpVelocity;
  }

  m_velocity.y += m_gravity * deltaTime;
  if (m_velocity.y > m_maxSpeed.y) m_velocity.y = m_maxSpeed.y;

  m_rect.y += m_velocity.y * deltaTime;

  m_onGround = false;

  for (const Block &block : blocks) {
    // Skip if not near block
    if (block.GetPositionX() < m_rect.x - MAX_BLOCK_LENGTH) continue;
    else if (block.GetTextureID() == m_skipCollisionBlock) continue;
    else if (block.GetPositionX() > m_rect.x + MAX_BLOCK_LENGTH) break;

    // Returns the size of the overlap
    collisionRect = GetCollisionRec(m_rect, block.GetRectangle());

    // Check if it doesn't collide
    if (collisionRect.width == 0) continue;

    blockRect = block.GetRectangle();

    // Check collision with bottom of player and block
    if (collisionRect.y != m_rect.y) {
      m_rect.y = blockRect.y - m_rect.height;
      m_onGround = true;
      m_velocity.y = 0;
      break;
    }
  }

  // Horizontal Movement
  if (m_inputState[GO_LEFT]) m_velocity.x -= m_acceleration * deltaTime;
  if (m_inputState[GO_RIGHT]) m_velocity.x += m_acceleration * deltaTime;

  if (m_velocity.x < 0) {
    m_facingLeft = true;
    m_currentTextureID = m_textureIDs[FACING_LEFT];
    m_velocity.x += m_friction * deltaTime;
    if (m_velocity.x < -m_maxSpeed.x) m_velocity.x = -m_maxSpeed.x;
    else if (m_velocity.x > 0) m_velocity.x = 0;
  }
  else if (m_velocity.x > 0) {
    m_facingLeft = false;
    m_currentTextureID = m_textureIDs[FACING_RIGHT];
    m_velocity.x -= m_friction * deltaTime;
    if (m_velocity.x > m_maxSpeed.x) m_velocity.x = m_maxSpeed.x;
    else if (m_velocity.x < 0) m_velocity.x = 0;
  }

  if (m_velocity.x != 0) {
    if (m_walkingTimer < 0) {
      PlaySound(m_sounds[WALKING]);
      m_walkingTimer = BASE_FPS / 2;
    }
    else m_walkingTimer -= deltaTime;
  }

  m_rect.x += m_velocity.x * deltaTime;

  for (const Block &block : blocks) {
    // Skip if not near block
    if (block.GetPositionX() < m_rect.x - MAX_BLOCK_LENGTH) continue;
    else if (block.GetTextureID() == m_skipCollisionBlock) continue;
    else if (block.GetPositionX() > m_rect.x + MAX_BLOCK_LENGTH) break;

    // Returns the size of the overlap
    collisionRect = GetCollisionRec(m_rect, block.GetRectangle());

    // Check if it doesn't collide
    if (collisionRect.width == 0) continue;

    blockRect = block.GetRectangle();

    // Check collision with the sides of the player and block
    if (collisionRect.x == m_rect.x) {
      // Left side
      m_rect.x = blockRect.x + blockRect.width;
      break;
    }
    else {
      // Right side
      m_rect.x = blockRect.x - m_rect.width;
      m_velocity.x = 0;
      break;
    }
  }
}


// Getters //

bool Entity::GetFacingLeft() const {return m_facingLeft;}


float Entity::GetPositionX() const {return m_rect.x;}
