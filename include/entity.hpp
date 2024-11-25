#ifndef HEADER_ENTITY
#define HEADER_ENTITY

#include <array>
#include <raylib.h>
#include "level.hpp"
#include "resource.hpp"

#define JUMP 0     // Input ID
#define GO_LEFT 1  // Input ID
#define GO_RIGHT 2 // Input ID
#define ATTACK_0 3 // Input ID
#define ATTACK_1 4 // Input ID

#define FACING_LEFT 0  // State ID
#define FACING_RIGHT 1 // State ID

#define DAMAGED 0  // Sound ID
#define JUMPED 1   // Sound ID
#define WALKING 2  // Sound ID


// Define //

class Entity {
  public:
    // Adjust volume of audio depending on distance
    void UpdateAudio(Camera2D camera);
    // Method - reduce immunity duration by deltaTime
    void UpdateImmunity(float deltaTime);
    // Method - uses m_inputState to update movement and check for collisions
    void UpdatePosition(const BlockList &blocks, float deltaTime);

    // Getter
    bool GetFacingLeft() const;
    // Getter
    float GetPositionX() const;

  protected:
    Rectangle m_rect = {0, 0, 0, 0}; // Position, size

    float m_walkingTimer = 0;
    std::array<Sound, 3> m_sounds;

    bool m_facingLeft = false;                  // State
    bool m_onGround = false;                    // State
    int m_skipCollisionBlock = ID_NULL_TEXTURE; // State
    int m_currentTextureID;                     // State
    float m_immunity;                           // State
    float m_maxImmunity;                        // State
    std::array<int, 2> m_textureIDs;            // State

    std::array<bool, 5> m_inputState = {false, false, false, false, false}; // Input

    const float m_friction = 0.9; // Movement
    const float m_gravity = 1.2;  // Movement
    float m_acceleration;         // Movement
    float m_jumpVelocity;         // Movement
    Vector2 m_maxSpeed = {0, 25}; // Movement
    Vector2 m_velocity = {0, 0};  // Movement
};


#endif
