#include <array>
#include <raylib.h>
#include <vector>
#include "file.hpp"
#include "game.hpp"
#include "resource.hpp"
#include "window.hpp"

// Functions //

void RenderHighscores(const Window &window, const ScoreEntryList &highScores) {
  std::string line = "";

  BeginDrawing();
    ClearBackground(SKYBLUE);
    DrawText("High Scores", window.width / 2 - MeasureText("High Scores", window.fontBig) / 2, window.spacing, window.fontBig, TEXTCOLOUR);

    for (int i = 0; i < (int)highScores.size(); i++) {
      line = highScores[i].name + ": " + std::to_string(highScores[i].score);
      DrawText(line.c_str(), window.width / 2 - MeasureText(line.c_str(), window.fontSmall) / 2, window.height / 3 + window.fontSmall * i, window.fontSmall, TEXTCOLOUR);
    }

    DrawText("R -> Reset High Scores", window.width / 2 - MeasureText("R -> Reset High Scores", window.fontSmall) / 2, window.height / 3 * 2, window.fontSmall, TEXTCOLOUR);
    DrawText("Esc -> Back To Menu", window.width / 2 - MeasureText("Esc -> Back To Menu", window.fontSmall) / 2, window.height / 3 * 2 + window.fontSmall, window.fontSmall, TEXTCOLOUR);
	EndDrawing();
}


void RenderMenu(const Window &window, bool confirmExit) {
  BeginDrawing();
    if (confirmExit) {
      // Render ConfirmExit
      ClearBackground(MAROON);
      DrawText("Confirm Exit?", window.width / 2 - MeasureText("Confirm Exit?", window.fontBig) / 2, window.spacing, window.fontBig, TEXTCOLOUR);
      DrawText("Y -> Exit", window.width / 2 - MeasureText("Y -> Exit", window.fontSmall) / 2, window.height / 2, window.fontSmall, TEXTCOLOUR);
      DrawText("N -> Back to Menu", window.width / 2 - MeasureText("N -> Back to Menu", window.fontSmall) / 2, window.height / 2 + window.fontSmall, window.fontSmall, TEXTCOLOUR);
    }
    else {
      // Render Menu
      ClearBackground(SKYBLUE);
      DrawText("Run!", window.width / 2 - MeasureText("Run!", window.fontBig) / 2, window.spacing, window.fontBig, TEXTCOLOUR);
      DrawText("G -> Play", window.width / 2 - MeasureText("G -> Play", window.fontSmall) / 2, window.height / 2, window.fontSmall, TEXTCOLOUR);
      DrawText("H -> High Scores", window.width / 2 - MeasureText("H -> High Scores", window.fontSmall) / 2, window.height / 2 + window.fontSmall, window.fontSmall, TEXTCOLOUR);
      DrawText("S -> Settings", window.width / 2 - MeasureText("S -> Settings", window.fontSmall) / 2, window.height / 2 + window.fontSmall * 2, window.fontSmall, TEXTCOLOUR);
      DrawText("ESC -> Quit", window.width / 2 - MeasureText("ESC -> Quit", window.fontSmall) / 2, window.height / 2 + window.fontSmall * 3, window.fontSmall, TEXTCOLOUR);
    }
  EndDrawing();
}


void RenderGameSelect(const Window &window, const SaveSlots &saves) {
  std::string line = "";

  BeginDrawing();
    ClearBackground(SKYBLUE);
    DrawText("Saves", window.width / 2 - MeasureText("Saves", window.fontBig) / 2, window.spacing, window.fontBig, TEXTCOLOUR);

    // Save Slots
    for (int i = 0; i < 3; i++) {
      DrawText(saves[i].name.c_str(), window.spacing + window.width / 3 * i, window.height / 3, window.fontSmall, TEXTCOLOUR);

      if (saves[i].str_level == "") DrawText("Empty", window.spacing + window.width / 3 * i, window.height / 3 + window.fontSmall, window.fontSmall, TEXTCOLOUR);
      else {
        DrawText(saves[i].str_level.c_str(), window.spacing + window.width / 3 * i, window.height / 3 + window.fontSmall, window.fontSmall, TEXTCOLOUR);
        DrawText(saves[i].str_score.c_str(), window.spacing + window.width / 3 * i, window.height / 3 + window.fontSmall * 2, window.fontSmall, TEXTCOLOUR);
      }
    }

    DrawText("Esc -> Back To Menu", window.width / 2 - MeasureText("Esc -> Back To Menu", window.fontSmall) / 2, window.height / 3 * 2, window.fontSmall, TEXTCOLOUR);
  EndDrawing();
}


void RenderSettings(const Window &window) {
  const std::array<std::string, 6> str_p1Keybinds = {"Player 1", "W - Jump", "A - Move Left", "D - Move Right", "Z - Weapon 1", "X - Weapon 2"};
  const std::array<std::string, 6> str_p2Keybinds = {"Player 2", "O - Jump", "K - Move Left", "; - Move Right", ", - Weapon 1", ". - Weapon 2"};
  const std::string str_menufps = "Menu FPS: " + std::to_string(window.menufps);
  const std::string str_gamefps = "Game FPS: " + std::to_string(window.gamefps);

  BeginDrawing();
    ClearBackground(SKYBLUE);
    DrawText("Settings", window.width / 2 - MeasureText("Settings", window.fontBig) / 2, window.spacing, window.fontBig, TEXTCOLOUR);

    // Player 1 Settings
    for (int i = 0; i < 6; i++) DrawText(str_p1Keybinds[i].c_str(), window.spacing, window.spacing * 2 + window.fontBig + window.fontSmall * i, window.fontSmall, TEXTCOLOUR);
    // Player 2 Settings
    for (int i = 0; i < 6; i++) DrawText(str_p2Keybinds[i].c_str(), window.width / 2 + window.spacing, window.spacing * 2 + window.fontBig + window.fontSmall * i, window.fontSmall, TEXTCOLOUR);

    DrawText(str_menufps.c_str(), window.spacing, window.spacing * 2 + window.fontBig + window.fontSmall * 7, window.fontSmall, TEXTCOLOUR);
    DrawText(str_gamefps.c_str(), window.spacing, window.spacing * 2 + window.fontBig + window.fontSmall * 8, window.fontSmall, TEXTCOLOUR);

    DrawText("F -> Toggle Fullscreen", window.width / 2 - MeasureText("F -> Toggle Fullscreen", window.fontSmall) / 2, window.height / 3 * 2, window.fontSmall, TEXTCOLOUR);
    DrawText("Esc -> Back To Menu", window.width / 2 - MeasureText("Esc -> Back To Menu", window.fontSmall) / 2, window.height / 3 * 2 + window.fontSmall, window.fontSmall, TEXTCOLOUR);
	EndDrawing();
}


void UpdateWindowVariables(Window &window) {
  if (window.width / 160 < window.height / 90) window.scale = window.width / 160;
  else window.scale = window.height / 90;
  window.spacing = window.scale * 3;
	window.fontBig = window.scale * 9;
	window.fontSmall = window.scale * 4;
}


void UpdateFullscreen(Window &window) {
  if (IsWindowFullscreen()) {
    // Exit fullscreen
    window.width = BASE_WINDOW_WIDTH;
    window.height = BASE_WINDOW_HEIGHT;
    ToggleFullscreen();
    SetWindowSize(window.width, window.height);
  }
  else {
    // Enter fullscreen
    window.monitor = GetCurrentMonitor();
    window.width = GetMonitorWidth(window.monitor);
    window.height = GetMonitorHeight(window.monitor);
    SetWindowSize(window.width, window.height);
    ToggleFullscreen();
  }

  UpdateWindowVariables(window);
}


// Main //

int main(void) {
  // Enable when building release
  SetTraceLogLevel(LOG_NONE);

	Window window;

  // Init Window
  InitWindow(window.width, window.height, "Run!");
  HideCursor();
  SetTargetFPS(window.menufps);
	SetExitKey(KEY_NULL);

  // Init Audio
  InitAudioDevice();
  SetMasterVolume(0.25);

  UpdateWindowVariables(window);
  window.monitor = GetCurrentMonitor();
  window.gamefps = GetMonitorRefreshRate(window.monitor);

  bool confirmExit = false;
  bool previousConfirmExit = confirmExit;
	AppStates appState = AppStates::Menu;
  AppStates previousAppState = appState;

  // Load Sounds
  Music backgroundMusic = LoadMusicStream("audio/bgm.wav");
  backgroundMusic.looping = true;
  SetMusicVolume(backgroundMusic, 0.25);
  PlayMusicStream(backgroundMusic);

  const SoundArray sounds = {LoadSound("audio/menu_1.ogg"),
                             LoadSound("audio/menu_2.ogg"),
                             LoadSound("audio/menu_3.ogg"),
                             LoadSound("audio/level_complete.ogg"),
                             LoadSound("audio/game_over.ogg"),
                             LoadSound("audio/gun.wav"),
                             LoadSound("audio/sword.wav"),
                             LoadSound("audio/player_damage.wav"),
                             LoadSound("audio/player_jump.wav"),
                             LoadSound("audio/player_walk.wav"),
                             LoadSound("audio/monster_damage.wav"),
                             LoadSound("audio/monster_jump.wav"),
                             LoadSound("audio/monster_walk.wav")
                            };

  // Load Textures
  const TextureArray textures = {LoadTexture("images/null.png"),
                                 LoadTexture("images/background.png"),
                                 LoadTexture("images/background_dark.png"),
                                 LoadTexture("images/air.png"),
                                 LoadTexture("images/dirt.png"),
                                 LoadTexture("images/fire.png"),
                                 LoadTexture("images/magma.png"),
                                 LoadTexture("images/gun.png"),
                                 LoadTexture("images/sword.png"),
                                 LoadTexture("images/player1_left.png"),
                                 LoadTexture("images/player1_right.png"),
                                 LoadTexture("images/player2_left.png"),
                                 LoadTexture("images/player2_right.png"),
                                 LoadTexture("images/enemy_left.png"),
                                 LoadTexture("images/enemy_right.png"),
                                 LoadTexture("images/enemy_strong_left.png"),
                                 LoadTexture("images/enemy_strong_right.png")
                                };

  // Highscore Data
  ScoreEntryList highscores;
  CheckFileExists(FILE_HIGHSCORES);

  // Save Files
  SaveSlots saves;
  CheckFileExists(SAVE_SLOT_0);
  CheckFileExists(SAVE_SLOT_1);
  CheckFileExists(SAVE_SLOT_2);
  saves[0].name += "1 -> Select Slot 1";
  saves[1].name += "2 -> Select Slot 2";
  saves[2].name += "3 -> Select Slot 3";

  // Main Loop
	while (appState != AppStates::Quitting) {
    UpdateMusicStream(backgroundMusic);

    // Check for menu change
    if (appState != previousAppState) PlaySound(sounds[ID_MENU_1]);
    else if (confirmExit != previousConfirmExit) PlaySound(sounds[ID_MENU_1]);
    previousAppState = appState;
    previousConfirmExit = confirmExit;

		switch (appState) {
      case AppStates::GameLoadSave:
        LoadSummary(SAVE_SLOT_0, saves[0].str_level, saves[0].str_score);
        LoadSummary(SAVE_SLOT_1, saves[1].str_level, saves[1].str_score);
        LoadSummary(SAVE_SLOT_2, saves[2].str_level, saves[2].str_score);
        appState = AppStates::GameSelectSave;

			case AppStates::GameSelectSave:
        if (IsKeyPressed(KEY_ESCAPE)) appState = AppStates::Menu;
        if (IsKeyPressed(KEY_ONE)) appState = RunGame(window, sounds, textures, backgroundMusic, SAVE_SLOT_0);
        else if (IsKeyPressed(KEY_TWO)) appState = RunGame(window, sounds, textures, backgroundMusic, SAVE_SLOT_1);
        else if (IsKeyPressed(KEY_THREE)) appState = RunGame(window, sounds, textures, backgroundMusic, SAVE_SLOT_2);
        RenderGameSelect(window, saves);
        break;

      case AppStates::HighscoreLoad:
        LoadHighscores(highscores);
        appState = AppStates::HighscoreShow;

      case AppStates::HighscoreShow:
        if (IsKeyPressed(KEY_ESCAPE)) appState = AppStates::Menu;
        if (IsKeyPressed(KEY_R)) {
          ResetHighscores(highscores);
          PlaySound(sounds[ID_MENU_2]);
        }
        RenderHighscores(window, highscores);
        break;

      case AppStates::GameToMenu:
        SetTargetFPS(window.menufps);
        SortHighscores();
        appState = AppStates::Menu;

			case AppStates::Menu:
        if (IsKeyPressed(KEY_Y) && confirmExit) appState = AppStates::Quitting;
				else if (IsKeyPressed(KEY_N) && confirmExit) confirmExit = false;
				else if (IsKeyPressed(KEY_ESCAPE)) confirmExit = true;
				else if (IsKeyPressed(KEY_G)) appState = AppStates::GameLoadSave;
        else if (IsKeyPressed(KEY_H)) appState = AppStates::HighscoreLoad;
				else if (IsKeyPressed(KEY_S)) appState = AppStates::Settings;
        RenderMenu(window, confirmExit);
				break;

			case AppStates::Quitting:
				break;

			case AppStates::Settings:
				if (IsKeyPressed(KEY_ESCAPE)) appState = AppStates::Menu;
        else if (IsKeyPressed(KEY_F)) {
          UpdateFullscreen(window);
          PlaySound(sounds[ID_MENU_3]);
        }
        RenderSettings(window);
		}

		if (WindowShouldClose()) appState = AppStates::Quitting;
	}

	// Unload
  UnloadMusicStream(backgroundMusic);
  for (const Sound &sound : sounds) UnloadSound(sound);
  for (const Texture &texture : textures) UnloadTexture(texture);
  CloseAudioDevice();
	CloseWindow();
  return 0;
}
