#ifndef HEADER_BACKGROUND
#define HEADER_BACKGROUND

#include <array>
#include <raylib.h>
#include "level.hpp"
#include "resource.hpp"


// Define //

class Background {
  public:
    // Method - draw to screen
    void Draw(const TextureArray &textures) const;
    // Method - update background texture
    void UpdateBackground(int level);
    // Method - moves background for parallax effect
    void UpdatePosition(Camera2D camera);

  private:
    Vector2 m_previousPosition = {0, 0}; // Position
    Vector2 m_renderPosition = {0, 0};   // Position

    const float m_parallax = 0.6; // Setting

    int m_textureID = ID_BACKGROUND; // State
};


#endif
