#ifndef HEADER_WINDOW
#define HEADER_WINDOW

#include <array>
#include <raylib.h>

#define BASE_WINDOW_WIDTH 1280
#define BASE_WINDOW_HEIGHT 720
#define BASE_FPS 60

#define TEXTCOLOUR BLACK


// Define //

struct Window {
  int monitor;
  int width = BASE_WINDOW_WIDTH;
	int height = BASE_WINDOW_HEIGHT;
	int scale;
  int spacing;
	int fontBig;
	int fontSmall;
	int menufps = BASE_FPS;
  int gamefps;
};


enum class AppStates {
  GameLoadSave,
  GameSelectSave,
  GameToMenu,
  HighscoreLoad,
  HighscoreShow,
  Menu,
  Quitting,
  Settings
};


#endif
