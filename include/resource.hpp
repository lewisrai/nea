#ifndef HEADER_RESOURCE
#define HEADER_RESOURCE

#include <array>
#include <raylib.h>

/*
  Sounds
  https://maygenko.itch.io/basic-rpg-sfx-by-maygenko
  https://leohpaz.itch.io/minifantasy-dungeon-sfx-pack
  https://sfxr.me/

  Textures
  Background clouds
  https://latenightcoffe.itch.io/2d-pixel-art-semi-realistic-clouds

  Player and Monster
  https://free-game-assets.itch.io/free-3-cyberpunk-sprites-pixel-art
  https://luizmelo.itch.io/monsters-creatures-fantasy
*/

// Sounds
#define ID_MENU_1 0
#define ID_MENU_2 1
#define ID_MENU_3 2
#define ID_LEVEL_COMPLETE 3
#define ID_GAME_OVER 4
#define ID_GUN_SHOOT 5
#define ID_SWORD_SWING 6
#define ID_PLAYER_DAMAGED 7
#define ID_PLAYER_JUMPED 8
#define ID_PLAYER_WALKING 9
#define ID_MONSTER_DAMAGED 10
#define ID_MONSTER_JUMPED 11
#define ID_MONSTER_WALKING 12

// Textures
#define ID_NULL_TEXTURE 0
#define ID_BACKGROUND 1
#define ID_BACKGROUND_DARK 2
#define ID_BLOCK_AIR 3
#define ID_BLOCK_DIRT 4
#define ID_BLOCK_FIRE 5
#define ID_BLOCK_MAGMA 6
#define ID_GUN 7
#define ID_SWORD 8
#define ID_PLAYER1_LEFT 9
#define ID_PLAYER1_RIGHT 10
#define ID_PLAYER2_LEFT 11
#define ID_PLAYER2_RIGHT 12
#define ID_ENEMY_LEFT 13
#define ID_ENEMY_RIGHT 14
#define ID_ENEMY_STRONG_LEFT 15
#define ID_ENEMY_STRONG_RIGHT 16

#define BACKGROUND_WIDTH 1600 // Texture width
#define BACKGROUND_HEIGHT 900 // Texture height


// Define //

typedef std::array<Sound, 13> SoundArray;


typedef std::array<Texture, 17> TextureArray;


#endif
