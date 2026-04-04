#include <Audio.hpp>
#include <Files.hpp>
#include <Weather.hpp>
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

void loadSettingsFile() {
  std::ifstream settingsFile("storage/settings.txt");
  if (settingsFile.is_open()) {
    settingsFile >> settingsMusicVolume;
    settingsFile >> settingsSoundFXVolume;
    settingsFile >> settingsWeatherFXVolume;
    settingsFile >> settingsWeatherActive;
  } else {
    settingsFile.close();
    std::ofstream settingsFile("storage/settings.txt");
    settingsMusicVolume = DEFAULT_MUSIC_VOLUME;
    settingsSoundFXVolume = DEFAULT_SOUNDFX_VOLUME;
    settingsWeatherFXVolume = DEFAULT_WEATHERFX_VOLUME;
    settingsFile << DEFAULT_MUSIC_VOLUME << "\n";     // Music
    settingsFile << DEFAULT_SOUNDFX_VOLUME << "\n";   // Sound FX
    settingsFile << DEFAULT_WEATHERFX_VOLUME << "\n"; // Weather FX
    settingsFile << 1;                                // Weather Active
  }
  settingsFile.close();
}

void updateFiles() {
  std::ofstream levelsFile("storage/levels.txt");
  levelsFile << maxLevelUnlocked;
  levelsFile.close();

  std::ofstream settingsFile("storage/settings.txt");
  settingsFile << settingsMusicVolume << "\n";
  settingsFile << settingsSoundFXVolume << "\n";
  settingsFile << settingsWeatherFXVolume << "\n";
  settingsFile << settingsWeatherActive;
  settingsFile.close();
}

void initFiles() {
  loadLevelsFile();
  loadSettingsFile();
}
