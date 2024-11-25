#include "file.hpp"

/*
  std::ifstream - read only
  std::ofstream - supports - std::ofstream::out - write
                           - std::ofstream::app - append at end of file
                           - std::ofstream::trunc - deletes all contents of file before write
                - creates new file if it doesnt exist
                - if not assigned to variable, file closes automatically

  std::getline(file, line) - returns true while there is a next line,
                           - line variable contains the string of the next line
                           - once called, automatically moves to next line
*/

// Functions //

void CheckFileExists(std::string fileName) {
  std::ifstream file(fileName);

  if (!file.good()) std::ofstream (fileName, std::ofstream::out);
  file.close();
}


void LoadHighscores(ScoreEntryList &highscores) {
  ScoreEntry entry;
  std::string line = "";
  std::ifstream file(FILE_HIGHSCORES);

  highscores.clear();
  while (std::getline(file, line)) {
    entry.name = line.substr(0, line.find(SEPARATOR));
    entry.score = stoi(line.substr(line.find(SEPARATOR) + 1));
    highscores.push_back(entry);
  }
  file.close();
}


void LoadProgress(std::string saveFile, int &level, float &score, float &maxPlayersHealth, std::array<int, 10> &upgrades) {
  std::string line = "";
  std::ifstream file(saveFile);

  if (std::getline(file, line)) level = stoi(line);
  if (std::getline(file, line)) score = stoi(line);
  if (std::getline(file, line)) maxPlayersHealth = stoi(line);
  for (int &upgrade : upgrades) if (std::getline(file, line)) upgrade = stoi(line);
  file.close();
}


void LoadSummary(std::string saveFile, std::string &str_level, std::string &str_score) {
  std::string line = "";
  std::ifstream file(saveFile);

  if (std::getline(file, line)) str_level = "Levels Completed: " + line;
  if (std::getline(file, line)) str_score = "Score: " + line;
  file.close();
}


void ResetHighscores(ScoreEntryList &highscores) {
  std::ofstream (FILE_HIGHSCORES, std::ofstream::trunc);

  highscores.clear();
}


void ResetSaveSlot(std::string saveFile) {std::ofstream (saveFile, std::ofstream::trunc);}


void SaveHighscore(std::string name, int score) {
  std::ofstream file(FILE_HIGHSCORES, std::ofstream::app);

  file << name << SEPARATOR << score << "\n";
  file.close();
}


void SaveProgress(std::string saveFile, int level, int score, int maxPlayersHealth, const std::array<int, 10> &upgrades) {
  std::ofstream file(saveFile, std::ofstream::trunc);

  file << level << "\n";
  file << score << "\n";
  file << maxPlayersHealth << "\n";
  for (const int &upgrade : upgrades) file << upgrade << "\n";
  file.close();
}


void SortHighscores() {
  std::array<ScoreEntry, MAX_HIGHSCORES> highscores;

  // Load current scores
  ScoreEntryList tempScores;
  LoadHighscores(tempScores);

  // Insertion sort
  for (ScoreEntry &entry : tempScores) {
    for (int i = 0; i < MAX_HIGHSCORES; i++) {
      if (entry.score > highscores[i].score) {
        for (int j = MAX_HIGHSCORES - 1; j > i; j--) highscores[j] = highscores[j - 1];
        highscores[i] = entry;
        break;
      }
    }
  }

  // Write sorted data to highscore file
  std::ofstream file(FILE_HIGHSCORES, std::ofstream::trunc);

  for (ScoreEntry &entry : highscores) if (entry.score != 0) file << entry.name << SEPARATOR << entry.score << "\n";
  file.close();
}
