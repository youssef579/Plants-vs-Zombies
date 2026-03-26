#include <Files.hpp>
#include <Audio.hpp>
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
    settingsFile >> Settings_MusicVolume;
    settingsFile >> Settings_SoundFXVolume;
  }
  else {
    settingsFile.close();
    std::ofstream settingsFile("storage/settings.txt");
    Settings_MusicVolume = DEFAULT_MUSIC_VOLUME;
    Settings_SoundFXVolume = DEFAULT_SOUNDFX_VOLUME;
    settingsFile << DEFAULT_MUSIC_VOLUME << "\n"; //Music
    settingsFile << DEFAULT_SOUNDFX_VOLUME; //Sound FX
  }
  settingsFile.close();
}


void updateFiles() {
  std::ofstream levelsFile("storage/levels.txt");
  levelsFile << maxLevelUnlocked;
  levelsFile.close();

  std::ofstream settingsFile("storage/settings.txt");
  settingsFile << Settings_MusicVolume << "\n";
  settingsFile << Settings_SoundFXVolume;
  settingsFile.close();
}

void initFiles() {
  loadLevelsFile();
  loadSettingsFile();
}
