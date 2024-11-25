#ifndef HEADER_FILE
#define HEADER_FILE

#include <array>
#include <fstream>
#include <string>
#include <vector>

#define FILE_HIGHSCORES "highscores.list" // Location
#define MAX_HIGHSCORES 5                  // Limit
#define SEPARATOR "@"                     // Field separator

#define SAVE_SLOT_0 "slot0.save" // Location
#define SAVE_SLOT_1 "slot1.save" // Location
#define SAVE_SLOT_2 "slot2.save" // Location


// Define //

struct SaveSlot {
  std::string name = "";
  std::string str_level = "";
  std::string str_score = "";
};


typedef std::array<SaveSlot, 3> SaveSlots;


struct ScoreEntry {
  std::string name = "";
  int score = 0;
};


typedef std::vector<ScoreEntry> ScoreEntryList;


// Functions //

// Creates a new file if it doesn't
void CheckFileExists(std::string fileName);

// Gets names and scores from highscore file
void LoadHighscores(ScoreEntryList &highscores);

// Gets the current level, score, health and player upgrades from save file
void LoadProgress(std::string saveFile, int &level, float &score, float &maxPlayersHealth, std::array<int, 10> &upgrades);

// Gets the current level in string data type
void LoadSummary(std::string saveFile, std::string &str_level, std::string &str_score);

// Deletes contents of highscore file and currently loaded highscores
void ResetHighscores(ScoreEntryList &highscores);

// Deletes contents of a save file
void ResetSaveSlot(std::string saveFile);

// Add score to the end of highscore file
void SaveHighscore(std::string name, int score);

// Saves the current level, score, health and player upgrades from save file
void SaveProgress(std::string saveFile, int level, int score, int maxPlayersHealth, const std::array<int, 10> &upgrades);

// Keep the top 5 scores and save to highscore file in descending order
void SortHighscores();


#endif
