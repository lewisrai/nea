#include "game.hpp"

// Functions //

void RenderChooseUpgrade(const Window &window, bool p0Selected, bool p1Selected, int level) {
  const std::array<std::string, 5> upgradeOptions = {"1 - Upgrade Sword", "2 - Upgrade Gun", "3 - Upgrade Movement", "4 - Upgrade Immunity", "5 - Upgrade Health + Damage Reduction"};
  const std::string str_level = "Level " + std::to_string(level) + " Completed";

  BeginDrawing();
    ClearBackground(SKYBLUE);
    DrawText(str_level.c_str(), window.width / 2 - MeasureText(str_level.c_str(), window.fontBig) / 2, window.spacing, window.fontBig, TEXTCOLOUR);

    for (int i = 0; i < 5; i++) DrawText(upgradeOptions[i].c_str(), window.spacing, window.spacing * 2 + window.fontBig + window.fontSmall * i, window.fontSmall, TEXTCOLOUR);

    if (p0Selected) DrawText("Player 1 - Upgrade Chosen", window.spacing, window.spacing * 2 + window.fontBig + window.fontSmall * 6, window.fontSmall, TEXTCOLOUR);
    else DrawText("Player 1 - Choose Using 1, 2, 3, 4, 5", window.spacing, window.spacing * 2 + window.fontBig + window.fontSmall * 6, window.fontSmall, TEXTCOLOUR);
    if (p1Selected) DrawText("Player 2 - Upgrade Chosen", window.spacing, window.spacing * 2 + window.fontBig + window.fontSmall * 7, window.fontSmall, TEXTCOLOUR);
    else DrawText("Player 2 - Choose Using 6, 7, 8, 9, 0", window.spacing, window.spacing * 2 + window.fontBig + window.fontSmall * 7, window.fontSmall, TEXTCOLOUR);

    DrawText("SPACE -> Save and Load Next Level", window.width / 2 - MeasureText("SPACE -> Save and Load Next Level", window.fontSmall) / 2, window.height / 3 * 2, window.fontSmall, TEXTCOLOUR);
    DrawText("Esc -> Save and Exit", window.width / 2 - MeasureText("Esc -> Save and Exit", window.fontSmall) / 2, window.height / 3 * 2 + window.fontSmall, window.fontSmall, TEXTCOLOUR);
  EndDrawing();
}


void RenderGame(const Window &window, Camera2D camera, const TextureArray &textures, const Background &background, const BlockList &blocks, const MonsterList &monsters, const PlayerArray &players, const ProjectileList &projectiles, const Tether &tether, const Wall &wall, int playersHealth, int score) {
  const float cameraLeftLimit = camera.target.x - BASE_WINDOW_WIDTH;
  const float cameraRightLimit = camera.target.x + BASE_WINDOW_WIDTH;
  const std::string str_playersHealth = "Health: " + std::to_string(playersHealth);
  const std::string str_monsterCount = "Monsters: " + std::to_string(monsters.size()) + " Left";
  const std::string str_score = "Score: " + std::to_string(score);

  BeginDrawing();
    // Render 2D World
    BeginMode2D(camera);
      background.Draw(textures);

      tether.Draw();

      for (const Monster &monster : monsters) monster.Draw(textures, cameraLeftLimit, cameraRightLimit);

      for (const Player &player : players) player.Draw(textures); // + weapons

      for (const Projectile &projectile : projectiles) projectile.Draw(cameraLeftLimit, cameraRightLimit);

      for (const Block &block : blocks) block.Draw(textures, cameraLeftLimit, cameraRightLimit);

      wall.Draw(textures);
    EndMode2D();

    // Render UI
    DrawText(str_playersHealth.c_str(), window.spacing, window.spacing, window.fontSmall, TEXTCOLOUR);
    DrawText(str_monsterCount.c_str(), window.spacing, window.spacing + window.fontSmall, window.fontSmall, TEXTCOLOUR);
    DrawText(str_score.c_str(), window.width / 2 - MeasureText(str_score.c_str(), window.fontBig) / 2, window.spacing, window.fontBig, TEXTCOLOUR);
    DrawText("Esc -> Exit, SPACE -> Pause", window.spacing, window.height - window.spacing - window.fontSmall, window.fontSmall, TEXTCOLOUR);
  EndDrawing();
}


void RenderPaused(const Window &window, bool confirmExit) {
  BeginDrawing();
    if (confirmExit) {
      // ConfirmExit
      ClearBackground(MAROON);
      DrawText("Confirm Exit?", window.width / 2 - MeasureText("Confirm Exit?", window.fontBig) / 2, window.spacing, window.fontBig, TEXTCOLOUR);
      DrawText("Y -> Exit", window.width / 2 - MeasureText("Y -> Exit", window.fontSmall) / 2, window.height / 2, window.fontSmall, TEXTCOLOUR);
      DrawText("N -> Back to Menu", window.width / 2 - MeasureText("N -> Back to Menu", window.fontSmall) / 2, window.height / 2 + window.fontSmall, window.fontSmall, TEXTCOLOUR);
    }
    else {
      // Paused
      ClearBackground(ORANGE);
      DrawText("Paused", window.width / 2 - MeasureText("Paused", window.fontBig) / 2, window.spacing, window.fontBig, TEXTCOLOUR);
      DrawText("SPACE -> Resume", window.width / 2 - MeasureText("SPACE -> Resume", window.fontSmall) / 2, window.height / 2, window.fontSmall, TEXTCOLOUR);
      DrawText("Esc -> Exit", window.width / 2 - MeasureText("Esc -> Exit", window.fontSmall) / 2, window.height / 2 + window.fontSmall, window.fontSmall, TEXTCOLOUR);
    }
  EndDrawing();
}


void RenderPlayerDied(const Window &window, std::string name, int level) {
  const std::string str_levelReached = "Level Reached: " + std::to_string(level);

  BeginDrawing();
    ClearBackground(ORANGE);
    DrawText("You Died", window.width / 2 - MeasureText("You Died", window.fontBig) / 2, window.spacing, window.fontBig, TEXTCOLOUR);

    DrawText(name.c_str(), window.width / 2 - MeasureText(name.c_str(), window.fontSmall) / 2, window.height / 3, window.fontSmall, TEXTCOLOUR);
    DrawText(str_levelReached.c_str(), window.width / 2 - MeasureText(str_levelReached.c_str(), window.fontSmall) / 2, window.height / 3 + window.fontSmall, window.fontSmall, TEXTCOLOUR);

    DrawText("R -> Randomise Name", window.width / 2 - MeasureText("R -> Randomise Name", window.fontSmall) / 2, window.height / 3 * 2, window.fontSmall, TEXTCOLOUR);
    DrawText("Y -> Save Score And Back To Menu", window.width / 2 - MeasureText("Y -> Save Score And Back To Menu", window.fontSmall) / 2, window.height / 3 * 2 + window.fontSmall, window.fontSmall, TEXTCOLOUR);
    DrawText("N -> Back To Menu", window.width / 2 - MeasureText("N -> Back To Menu", window.fontSmall) / 2, window.height / 3 * 2 + window.fontSmall * 2, window.fontSmall, TEXTCOLOUR);
  EndDrawing();
}


AppStates RunGame(const Window &window, const SoundArray &sounds, const TextureArray &textures, Music backgroundMusic, std::string saveFile) {
  // Init Variables
  const std::array<std::string, 7> nameOptions = {"Apple", "Banana", "Coconut", "Dragonfruit", "Elderberry", "Fig", "Grape"};
  std::string name = "noname";

  bool p0Selected = false;
  bool p1Selected = false;

  bool confirmExit = false;
  bool previousConfirmExit = confirmExit;
  float deltaTime = 0;
  GameStates gameState = GameStates::LoadLevel;
  Rectangle endPoint = {0, 0, 0, 0};

  Background background;
  BlockList blocks;
  CameraView cameraView(window);
  MonsterList monsters;
  ProjectileList projectiles;
  Tether tether;
  Wall wall;

  // New save
  float maxPlayersHealth = 100;
  float playersHealth = 0;
  float score = 0;
  int level = 0;
  PlayerArray players = {Player(sounds, true, KEY_W, KEY_A, KEY_D, KEY_Q, KEY_E), Player(sounds, false, KEY_O, KEY_K, KEY_SEMICOLON, KEY_I, KEY_P)};
  std::array<int, 10> upgrades = {0, -1, 0, 0, 0, 0, -1, 0, 0, 0};

  LoadProgress(saveFile, level, score, maxPlayersHealth, upgrades);
  for (Player &player : players) player.ApplyUpgrades(sounds, upgrades);
  SetTargetFPS(window.gamefps);

  // Game Loop
  while (true) {
    UpdateMusicStream(backgroundMusic);

    // Check for menu change
    if (confirmExit != previousConfirmExit) PlaySound(sounds[ID_MENU_1]);
    previousConfirmExit = confirmExit;

    switch (gameState) {
      case GameStates::LevelComplete:
        PlaySound(sounds[ID_LEVEL_COMPLETE]);
        score += level * 20;
        p0Selected = false;
        p1Selected = false;
        gameState = GameStates::ChooseUpgrade;

      case GameStates::ChooseUpgrade:
        if (IsKeyPressed(KEY_ESCAPE)) {
          SaveProgress(saveFile, level, score, maxPlayersHealth, upgrades);
          return AppStates::GameToMenu;
        }
        else if (IsKeyPressed(KEY_SPACE)) {
          for (Player &player : players) player.ApplyUpgrades(sounds, upgrades);
          SaveProgress(saveFile, level, score, maxPlayersHealth, upgrades);
          gameState = GameStates::LoadLevel;
        }

        // Check Player 0 has chosen an upgrade
        if (!p0Selected) {
          for (int i = 0; i < 4; i++) {
            if (IsKeyPressed(i + 49)) {
              PlaySound(sounds[ID_MENU_3]);
              upgrades[i] += 1;
              p0Selected = true;
              break;
            }
          }
          if (IsKeyPressed(KEY_FIVE)) {
            PlaySound(sounds[ID_MENU_3]);
            maxPlayersHealth += 10;
            upgrades[4] += 1;
            p0Selected = true;
          }
        }

        // Check Player 1 has chosen an upgrade
        if (!p1Selected) {
          for (int i = 5; i < 9; i++) {
            if (IsKeyPressed(i + 49)) {
              PlaySound(sounds[ID_MENU_3]);
              upgrades[i] += 1;
              p1Selected = true;
              break;
            }
          }
          if (IsKeyPressed(KEY_ZERO)) {
            PlaySound(sounds[ID_MENU_3]);
            maxPlayersHealth += 10;
            upgrades[9] += 1;
            p1Selected = true;
          }
        }

        RenderChooseUpgrade(window, p0Selected, p1Selected, level);
        break;

      case GameStates::LoadLevel:
        PlaySound(sounds[ID_MENU_1]);
        level += 1;
        background.UpdateBackground(level);
        endPoint = GenerateLevelData(blocks, level);
        GenerateEnemies(sounds, blocks, monsters, level);
        for (Player &player : players) player.Reset();
        playersHealth = maxPlayersHealth;
        projectiles.clear();
        wall.Reset(level);
        gameState = GameStates::Running;
        break;

      case GameStates::Paused:
        if (IsKeyPressed(KEY_Y) && confirmExit) return AppStates::GameToMenu;
        else if (IsKeyPressed(KEY_N) && confirmExit) confirmExit = false;
        else if (IsKeyPressed(KEY_ESCAPE)) confirmExit = true;
        else if (IsKeyPressed(KEY_SPACE)) {
          PlaySound(sounds[ID_MENU_1]);
          gameState = GameStates::Running;
        }
        RenderPaused(window, confirmExit);
        break;

      case GameStates::PlayerDied:
        ResetSaveSlot(saveFile);
        if (IsKeyPressed(KEY_Y)) {
          SaveHighscore(name, score);
          return AppStates::GameToMenu;
        }
        else if (IsKeyPressed(KEY_N)) return AppStates::GameToMenu;
        else if (IsKeyPressed(KEY_R)) {
          PlaySound(sounds[ID_MENU_3]);
          name = nameOptions[GetRandomValue(0, 6)];
        }
        RenderPlayerDied(window, name, level);
        break;

      case GameStates::Running:
        // Menu
        if (IsKeyPressed(KEY_ESCAPE)) {
          gameState = GameStates::Paused;
          confirmExit = true;
        }
        else if (IsKeyPressed(KEY_SPACE)) {
          PlaySound(sounds[ID_MENU_1]);
          gameState = GameStates::Paused;
        }

        deltaTime = BASE_FPS * GetFrameTime();

        // Players
        for (Player &player : players) {
          player.UpdateInput();
          player.UpdatePosition(blocks, deltaTime);
          player.UpdateImmunity(deltaTime);
          player.UpdateAudio(cameraView.GetCamera());
          player.UpdateAttack(deltaTime, projectiles);
          player.UpdateReceived(blocks, monsters, wall, playersHealth);
          if (CheckCollisionRecs(player.GetRectangle(), endPoint)) gameState = GameStates::LevelComplete; // End of level reached
          else if (player.GetCentre().y > BORDER_VERTICAL + LEVEL_PADDING) playersHealth = 0; // Out of bounds
        }

        // Projectiles
        for (Projectile &projectile : projectiles) {
          projectile.UpdateMovement(deltaTime);
          projectile.UpdateCollision(blocks);
        }

        // Monsters
        for (Monster &monster : monsters) {
          monster.UpdateAI(blocks, players);
          monster.UpdatePosition(blocks, deltaTime);
          monster.UpdateAudio(cameraView.GetCamera());
          monster.UpdateImmunity(deltaTime);
          monster.UpdateReceived(players, projectiles, wall);
        }

        projectiles = RemoveFalseProjectiles(projectiles);
        monsters = RemoveDeadEnemies(monsters, score);

        // Tether
        tether.Update(players, playersHealth);

        if (playersHealth <= 0) {
          PlaySound(sounds[ID_GAME_OVER]);
          gameState = GameStates::PlayerDied;
        }

        // Wall
        wall.Update(cameraView.GetCamera(), deltaTime);

        // Render
        cameraView.Update(players);
        background.UpdatePosition(cameraView.GetCamera());
        RenderGame(window, cameraView.GetCamera(), textures, background, blocks, monsters, players, projectiles, tether, wall, playersHealth, score);
    }

    if (WindowShouldClose()) return AppStates::Quitting;
  }
}
