#include <Files.hpp>
#include <Audio.hpp>
#include <fstream>
#include <globals.hpp>
#include <Weather.hpp>
#include <LevelManager.hpp>

// Level Selector
int maxLevelUnlocked = 1, levelSelectorCurrentPage = 1;
const int ACTUAL_MAX_LEVELS = 1; // how many level files actually exist

float Settings::musicVolume;
float Settings::soundFXVolume;
float Settings::weatherFXVolume;
int Settings::timeModifier; // not saved in storage
bool Settings::weatherActive;
bool Settings::fullscreen;

Settings settings;

void loadPlayerData() {
  std::ifstream file("storage/playerData.txt");

  if (file.is_open()) {
    file >> maxLevelUnlocked;
  } else { // levels.txt not found
    file.close();

    std::ofstream file("storage/playerData.txt");
    file << 1;
  }
  file.close();
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

void loadLevelsFiles() {
  for (int levelIdx = 1; levelIdx <= ACTUAL_MAX_LEVELS; levelIdx++) {
    std::string path = "storage/level_" + std::to_string(levelIdx) + ".txt";
    std::ifstream file(path);

    
    LevelManager::Level *newLevel = new LevelManager::Level;
    std::string input;
    int inputI;
    file >> input; // Location
    if      (input == "Day")   newLevel->location = LevelManager::Level::Day;
    else if (input == "Night") newLevel->location = LevelManager::Level::Night;
    else if (input == "Pool")  newLevel->location = LevelManager::Level::Pool;
    else if (input == "Roof")  newLevel->location = LevelManager::Level::Roof;

    file >> inputI; // number of waves
    newLevel->numberOfWaves = inputI;

    Array<float> delays;
    float inputF;
    for (int i = 0; i < newLevel->numberOfWaves; i++) { // Wave Delays
      file >> inputF;
      delays.push(inputF);
    }

    Array<float> durations;
    for (int i = 0; i < newLevel->numberOfWaves; i++) { // Wave Durations
      file >> inputF;
      durations.push(inputF);
    }


    Array<int> numberOfZombies;
    for (int i = 0; i < newLevel->numberOfWaves; i++) { // wave . Number of zombies
      file >> inputI;
      numberOfZombies.push(inputI);
    }

    for (int i = 0; i < newLevel->numberOfWaves; i++) {
      LevelManager::Level::Wave *newWave = new LevelManager::Level::Wave;
      newWave->delay = delays[i];
      newWave->duration = durations[i];
      for (int z = 0; z < numberOfZombies[i]; z++) {
        file >> input; // zombie type

        if      (input == "Regular")    newWave->zombieTypes.push(Zombie::Type::Regular);
        else if (input == "Conehead")   newWave->zombieTypes.push(Zombie::Type::Conehead);
        else if (input == "Buckethead") newWave->zombieTypes.push(Zombie::Type::Buckethead);
        else if (input == "Flag")       newWave->zombieTypes.push(Zombie::Type::Flag);
        else if (input == "Screendoor") newWave->zombieTypes.push(Zombie::Type::Screendoor);
      }

      newLevel->waves.push(newWave);
    }

    levelManager.levels.push(newLevel);


    file.close();

  }
}



void updateFiles() {
  std::ofstream file("storage/playerData.txt");
  file << maxLevelUnlocked;
  file.close();

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
  loadPlayerData();
  loadSettingsFile();
  loadLevelsFiles();
}
