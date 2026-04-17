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

void loadSettingsFile() {
  std::ifstream settingsFile("storage/settings.txt");
  if (settingsFile.is_open()) {
    // Load settings from file
    settingsFile >> settings.musicVolume;
    settingsFile >> settings.soundFXVolume;
    settingsFile >> settings.weatherFXVolume;
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
    settings.weatherActive   =  settings.DEFAULT.weatherActive;
    settings.fullscreen   =  settings.DEFAULT.fullscreen;
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
