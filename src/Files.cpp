#include "Grid.hpp"
#include <Zombies/Zombie.hpp>
#include <Files.hpp>
#include <Audio.hpp>
#include <fstream>
#include <globals.hpp>
#include <Weather.hpp>
#include <string>
// Level Selector
int maxLevelUnlocked = 1, levelSelectorCurrentPage = 1;

float Settings::musicVolume;
float Settings::soundFXVolume;
float Settings::weatherFXVolume;
int Settings::timeModifier; // not saved in storage
bool Settings::weatherActive;
bool Settings::fullscreen;

Settings settings;

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

void loadLevelFile(int level) { // Rename the above function to something else to avoid confusion
  std::ifstream levelFile("storage/level_" + std::to_string(level) + ".txt");

  int nWaves;
  levelFile >> nWaves;

  for (int i = 0; i < nWaves; i++) {
    int nZombies, startTimer;
    levelFile >> nZombies >> startTimer;

    for (int j = 0; j < nZombies; j++) {
      int zombieTypeIndex;
      levelFile >> zombieTypeIndex;

      int row = randomRange(0, ROWS_NUMBER - 1), offset = randomRange(250, 400);
      Zombie::createZombie(
        grid[row][8].rectangle.getGlobalBounds().getCenter().x + offset,
        grid[row][8].rectangle.getGlobalBounds().getCenter().y,
        (Zombie::Type)zombieTypeIndex, // index 0 is the first type, 1 the second type and so on
        row,
        startTimer
      );
    }
  }
}

void loadSettingsFile() {
  std::ifstream settingsFile("storage/settings.txt");
  if (settingsFile.is_open()) {
    // Load settings from file
    settingsFile >> settings.musicVolume;
    settingsFile >> settings.soundFXVolume;
    settingsFile >> settings.weatherFXVolume;
    settings.timeModifier = 1;
    settingsFile >> settings.weatherActive;
    settingsFile >> settings.fullscreen;
  }
  else {
    settingsFile.close();
    std::ofstream settingsFile("storage/settings.txt");
    // Load settings from default values
    settings.musicVolume     =  settings.DEFAULT.musicVolume;
    settings.soundFXVolume   =  settings.DEFAULT.soundFXVolume;
    settings.weatherFXVolume =  settings.DEFAULT.weatherFXVolume;
    settings.timeModifier    =  settings.DEFAULT.timeModifier;
    settings.weatherActive   =  settings.DEFAULT.weatherActive;
    settings.fullscreen      =  settings.DEFAULT.fullscreen;
    settingsFile << settings.DEFAULT.musicVolume << "\n";     //Music
    settingsFile << settings.DEFAULT.soundFXVolume << "\n";   //Sound FX
    settingsFile << settings.DEFAULT.weatherFXVolume << "\n"; //Weather FX
    settingsFile << settings.DEFAULT.weatherActive << "\n";   //Weather Active
    settingsFile << settings.DEFAULT.fullscreen;              //Fullscreen
  }
  settingsFile.close();
}


void updateFiles() {
  std::ofstream levelsFile("storage/levels.txt");
  levelsFile << maxLevelUnlocked;
  levelsFile.close();

  // Write settings to file
  std::ofstream settingsFile("storage/settings.txt");
  settingsFile << settings.musicVolume << "\n";
  settingsFile << settings.soundFXVolume << "\n";
  settingsFile << settings.weatherFXVolume << "\n";
  settingsFile << settings.weatherActive << "\n";
  settingsFile << settings.fullscreen;
  settingsFile.close();
}

void initFiles() {
  loadLevelsFile();
  loadSettingsFile();
}
