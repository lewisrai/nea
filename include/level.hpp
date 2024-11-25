#ifndef HEADER_LEVEL
#define HEADER_LEVEL

#include <array>
#include <raylib.h>
#include <vector>
#include "resource.hpp"
#include "window.hpp"

#define BLOCK_SIZE 48                   // Tile size
#define BORDER_HORIZONTAL 9500          // Level width
#define BORDER_VERTICAL 2500            // Level height
#define LEVEL_PADDING 500               // Border spacing
#define MAX_BLOCK_LENGTH BLOCK_SIZE * 7 // Max length

#define DIFFICULTY_INCREASE_1 4 // First increase in difficulty
#define DIFFICULTY_INCREASE_2 9 // Second increase in difficulty

#define WALL_BEHIND BLOCK_SIZE * 30
#define WALL_HEIGHT BLOCK_SIZE * 40


// Block Define //

class Block {
  public:
    // Init
    Block(int level, int textureID, Rectangle rect);

    // Method - checks if in view of camera before drawing to screen
    void Draw(const TextureArray &textures, float cameraLimitLeft, float cameraLimitRight) const;

    // Getter
    float GetDamage() const;
    // Getter
    float GetPositionX() const;
    // Getter
    int GetTextureID() const;
    // Getter
    Rectangle GetRectangle() const;

  private:
    const Rectangle m_rect; // Position, size

    const int m_textureID; // State

    float m_damage = 0; // Stat
};


typedef std::vector<Block> BlockList;


// Block Functions //

// Randomly generates path with features
Rectangle GenerateLevelData(BlockList &blocks, int level);


// Wall Define //

class Wall {
  public:
    // Method - draw to screen
    void Draw(const TextureArray &textures) const;
    // Method - match height of camera + move forwards
    void Update(Camera2D camera, float deltaTime);

    // Getter
    float GetFrontPositionX() const;

    // Setter
    void IncreaseSpeed();
    // Setter
    void Reset(int level);

  private:
    Rectangle m_rect = {0, 0, BLOCK_SIZE, WALL_HEIGHT}; // Position, size

    int m_textureID = ID_BLOCK_FIRE; // State

    float m_multiplier = 1; // Movement
    float m_speed = 0;      // Movement
};


#endif
