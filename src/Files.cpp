#include <Files.hpp>
#include <Audio.hpp>
#include <fstream>
#include <globals.hpp>
#include <Weather.hpp>
// Level Selector
int maxLevelUnlocked = 1, levelSelectorCurrentPage = 1;

float Settings::musicVolume;
float Settings::soundFXVolume;
float Settings::weatherFXVolume;
bool Settings::weatherActive;

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

void loadSettingsFile() {
  std::ifstream settingsFile("storage/settings.txt");
  if (settingsFile.is_open()) {
    // Load settings from file
    settingsFile >> settings.musicVolume;
    settingsFile >> settings.soundFXVolume;
    settingsFile >> settings.weatherFXVolume;
    settingsFile >> settings.weatherActive;
  }
  else {
    settingsFile.close();
    std::ofstream settingsFile("storage/settings.txt");
    // Load settings from default values
    settings.musicVolume     =  settings.default.musicVolume;
    settings.soundFXVolume   =  settings.default.soundFXVolume;
    settings.weatherFXVolume =  settings.default.weatherFXVolume;
    settings.weatherActive   =  settings.default.weatherActive;
    settingsFile << settings.default.musicVolume << "\n";     //Music
    settingsFile << settings.default.soundFXVolume << "\n";   //Sound FX
    settingsFile << settings.default.weatherFXVolume << "\n"; //Weather FX
    settingsFile << settings.default.weatherActive;           //Weather Active
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
  settingsFile << settings.weatherActive;
  settingsFile.close();
}

void initFiles() {
  loadLevelsFile();
  loadSettingsFile();
}
