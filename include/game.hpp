#ifndef HEADER_GAME
#define HEADER_GAME

#include <array>
#include <raylib.h>
#include <string>
#include <vector>
#include "background.hpp"
#include "camera.hpp"
#include "monster.hpp"
#include "file.hpp"
#include "level.hpp"
#include "player.hpp"
#include "resource.hpp"
#include "weapon.hpp"
#include "window.hpp"


// Define //

enum class GameStates {
  LevelComplete,
  ChooseUpgrade,
  LoadLevel,
  Paused,
  PlayerDied,
  Running
};


// Functions //

// Game Loop
AppStates RunGame(const Window &window, const SoundArray &sounds, const TextureArray &textures, Music backgroundMusic, std::string saveFile);


#endif
