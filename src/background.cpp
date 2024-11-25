#include "background.hpp"

// Public Methods //

void Background::Draw(const TextureArray &textures) const {
  // Renders a 4x4 grid with the background texture
  ClearBackground(RAYWHITE);

  for (int x = m_renderPosition.x - BACKGROUND_WIDTH * 2; x < m_renderPosition.x + BACKGROUND_WIDTH + 1; x += BACKGROUND_WIDTH) {
    for (int y = m_renderPosition.y - BACKGROUND_HEIGHT * 2; y < m_renderPosition.y + BACKGROUND_HEIGHT + 1; y += BACKGROUND_HEIGHT) {
	    DrawTexture(textures[m_textureID], x, y, WHITE);
	  }
  }
}


void Background::UpdateBackground(int level) {
  // Change background when difficulty increases
  if (level > DIFFICULTY_INCREASE_2) m_textureID = ID_BACKGROUND_DARK;
}


void Background::UpdatePosition(Camera2D camera) {
  const Vector2 change = {camera.target.x - m_previousPosition.x, camera.target.y - m_previousPosition.y};
  m_previousPosition = camera.target;

  // Moves background by less than the player has moved
  m_renderPosition.x += change.x * m_parallax;
  m_renderPosition.y += change.y * m_parallax;

  // Moves grid of background textures if player crosses a texture
  if (m_renderPosition.x - BACKGROUND_WIDTH / 2 > camera.target.x) m_renderPosition.x -= BACKGROUND_WIDTH;
  else if (m_renderPosition.x + BACKGROUND_WIDTH / 2 < camera.target.x) m_renderPosition.x += BACKGROUND_WIDTH;
  if (m_renderPosition.y - BACKGROUND_HEIGHT / 2 > camera.target.y) m_renderPosition.y -= BACKGROUND_HEIGHT;
  else if (m_renderPosition.y + BACKGROUND_HEIGHT / 2 < camera.target.y) m_renderPosition.y += BACKGROUND_HEIGHT;
}
