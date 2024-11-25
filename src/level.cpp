#include "level.hpp"

// Block Init //

Block::Block(int level, int textureID, Rectangle rect) : m_rect(rect), m_textureID(textureID) {
  if (m_textureID != ID_BLOCK_MAGMA) return;

  if (level > DIFFICULTY_INCREASE_2) m_damage = 15.0 / BASE_FPS;
  else m_damage = 10.0 / BASE_FPS;
}


// Public Methods //

void Block::Draw(const TextureArray &textures, float cameraLimitLeft, float cameraLimitRight) const {
  //Skip if not near players
  if (m_rect.x < cameraLimitLeft) return;
  else if (m_rect.x > cameraLimitRight) return;

  for (int x = 0; x < m_rect.width; x += BLOCK_SIZE) DrawTexture(textures[m_textureID], m_rect.x + x, m_rect.y, WHITE);
}


// Getters //

float Block::GetDamage() const {return m_damage;}


float Block::GetPositionX() const {return m_rect.x;}


int Block::GetTextureID() const {return m_textureID;}


Rectangle Block::GetRectangle() const {return m_rect;}


// Functions //

// After adding terrain feature, returns new last block position of level for other obstacles

// Flat terrain - enemies usually spawn since block size >= 3
Vector2 AddTerrainFlat(BlockList &blocks, Vector2 currentPosition, int maxLength = 6) {
  const float length = BLOCK_SIZE * GetRandomValue(3, maxLength);

  blocks.push_back(Block(0, ID_BLOCK_DIRT, {currentPosition.x, currentPosition.y - BLOCK_SIZE, length, BLOCK_SIZE}));
  currentPosition.x += length;

  return currentPosition;
}

// Terrain slopes downwards
Vector2 AddTerrainHillDown(BlockList &blocks, Vector2 currentPosition) {
  if (currentPosition.y > BORDER_VERTICAL - LEVEL_PADDING) return currentPosition;
  const float length = BLOCK_SIZE * GetRandomValue(2, 5);

  currentPosition = AddTerrainFlat(blocks, currentPosition, 3);
  for (int i = 0; i < GetRandomValue(2, 4); i++) {
    blocks.push_back(Block(0, ID_BLOCK_DIRT, {currentPosition.x - BLOCK_SIZE, currentPosition.y, length, BLOCK_SIZE}));
    currentPosition.x += length - BLOCK_SIZE;
    currentPosition.y += BLOCK_SIZE;
  }

  return currentPosition;
}

// Terrain slopes upwards
Vector2 AddTerrainHillUp(BlockList &blocks, Vector2 currentPosition) {
  if (currentPosition.y < -BORDER_VERTICAL + LEVEL_PADDING) return currentPosition;
  const float length = BLOCK_SIZE * GetRandomValue(2, 5);

  currentPosition = AddTerrainFlat(blocks, currentPosition, 3);
  for (int i = 0; i < GetRandomValue(2, 4); i++) {
    blocks.push_back(Block(0, ID_BLOCK_DIRT, {currentPosition.x - BLOCK_SIZE, currentPosition.y - BLOCK_SIZE * 2, length, BLOCK_SIZE}));
    currentPosition.x += length - BLOCK_SIZE;
    currentPosition.y -= BLOCK_SIZE;
  }

  return currentPosition;
}

// Gaps that players can fall through but monsters can't
Vector2 AddTerrainHoles(BlockList &blocks, Vector2 currentPosition) {
  const float lengthGap = BLOCK_SIZE * GetRandomValue(5, 6);
  const float lengthPlatform = BLOCK_SIZE * GetRandomValue(2, 3);

  for (int i = 0; i < GetRandomValue(2, 3); i++) {
    blocks.push_back(Block(0, ID_BLOCK_DIRT, {currentPosition.x, currentPosition.y - BLOCK_SIZE, lengthPlatform, BLOCK_SIZE}));
    currentPosition.x += lengthPlatform;
    blocks.push_back(Block(0, ID_BLOCK_AIR, {currentPosition.x, currentPosition.y - BLOCK_SIZE, lengthGap, BLOCK_SIZE}));
    currentPosition.x += lengthGap;
  }

  return currentPosition;
}


Vector2 AddTerrainMagma(BlockList &blocks, Vector2 currentPosition, int level) {
  float length = 0;

  for (int i = 0; i < GetRandomValue(4, 6); i++) {
    length = BLOCK_SIZE * GetRandomValue(1, 2);
    blocks.push_back(Block(level, (GetRandomValue(0, 1)) ? ID_BLOCK_MAGMA : ID_BLOCK_DIRT, {currentPosition.x, currentPosition.y - BLOCK_SIZE, length, BLOCK_SIZE}));
    currentPosition.x += length;
  }

  return currentPosition;
}


Rectangle GenerateLevelData(BlockList &blocks, int level) {
  int randomValue = 0;
  Vector2 currentPosition = {-BORDER_HORIZONTAL + LEVEL_PADDING, 0};
  blocks.clear();

  currentPosition = AddTerrainFlat(blocks, currentPosition, 3);
  while (currentPosition.x < BORDER_HORIZONTAL - LEVEL_PADDING) {
    randomValue = GetRandomValue(0, 59);
    if (randomValue < 12) currentPosition = AddTerrainHillDown(blocks, currentPosition);
    else if (randomValue < 24) currentPosition = AddTerrainHillUp(blocks, currentPosition);
    else if (randomValue < 30 && level > DIFFICULTY_INCREASE_1) currentPosition = AddTerrainMagma(blocks, currentPosition, level);
    else if (randomValue < 36 && level > DIFFICULTY_INCREASE_2) currentPosition = AddTerrainHoles(blocks, currentPosition);
    else currentPosition = AddTerrainFlat(blocks, currentPosition);
  }
  AddTerrainFlat(blocks, currentPosition, 3);

  // EndPoint
  return {currentPosition.x, currentPosition.y - 4 * BLOCK_SIZE, 3 * BLOCK_SIZE, 3 * BLOCK_SIZE};
}


// Wall Public Methods //

void Wall::Draw(const TextureArray &textures) const {
  for (int y = m_rect.y; y < m_rect.y + m_rect.height; y += BLOCK_SIZE) DrawTexture(textures[m_textureID], m_rect.x, y, WHITE);

  DrawRectangleRec({m_rect.x - WALL_BEHIND, m_rect.y, WALL_BEHIND, m_rect.height}, {0, 0, 0, 128});
}


void Wall::Update(Camera2D camera, float deltaTime) {
  // Update x
  m_rect.x += m_speed * deltaTime;

  // Update y
  if (m_rect.y + WALL_HEIGHT / 2 - BLOCK_SIZE > camera.target.y) m_rect.y -= BLOCK_SIZE;
  else if (m_rect.y + WALL_HEIGHT / 2 + BLOCK_SIZE < camera.target.y) m_rect.y += BLOCK_SIZE;
}


// Getters

float Wall::GetFrontPositionX() const {return m_rect.x + m_rect.width;}


// Setters //

void Wall::IncreaseSpeed() {m_speed *= m_multiplier;}


void Wall::Reset(int level) {
  m_rect.x = -BORDER_HORIZONTAL;

  if (level > DIFFICULTY_INCREASE_2) {
    m_speed = 7;
    m_multiplier = 1.25;
  }
  else if (level > DIFFICULTY_INCREASE_1) {
    m_speed = 4;
    m_multiplier = 1.1;
  }
  else m_speed = 2;
}
