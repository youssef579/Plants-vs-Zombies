#include <Files.hpp>
#include <fstream>
#include <globals.hpp>

// Level Selector
int maxLevelUnlocked = 1, levelSelectorCurrentPage = 1;

void loadLevelsFile() {
  std::ifstream levelsFile("storage/levels.txt");

  if (levelsFile.is_open()) {
    levelsFile >> maxLevelUnlocked;
  } else { // levels.txt not found
    levelsFile.close();

    std::ofstream levelsFile("storage/levels.txt");
    levelsFile << 1;
  }
  levelsFile.close();
}

void updateFiles() {
  std::ofstream levelsFile("storage/levels.txt");

  levelsFile << maxLevelUnlocked;
  levelsFile.close();
}

void initFiles() {
  loadLevelsFile();
  // load settings file
}
