#include <Files.hpp>
#include <Audio.hpp>
#include <fstream>
#include <globals.hpp>
#include <Weather.hpp>
#include <LevelManager.hpp>
#include <BackgroundManager.hpp>
#include <iostream>

// Level Selector
int maxLevelUnlocked = 1, levelSelectorCurrentPage = 1;
const int ACTUAL_MAX_LEVELS = 7; // how many level files actually exist

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
    bool inputB;

    file >> input; // Location
    if      (input == "Day")   newLevel->location = LevelManager::Level::Day;
    else if (input == "Night") newLevel->location = LevelManager::Level::Night;
    else if (input == "Pool")  newLevel->location = LevelManager::Level::Pool;
    else if (input == "Roof")  newLevel->location = LevelManager::Level::Roof;

    file >> input; // Reward
    if      (input == "SUN_FLOWER")      newLevel->reward = SUN_FLOWER;
    else if (input == "WALLNUT")         newLevel->reward = WALLNUT;
    else if (input == "TALLNUT")         newLevel->reward = TALLNUT;
    else if (input == "SNOWPEASHOOTER")  newLevel->reward = SNOWPEASHOOTER;
    else if (input == "REPEATERPEA")     newLevel->reward = REPEATERPEA;
    else if (input == "CHERRYBOMB")      newLevel->reward = CHERRYBOMB;
    else if (input == "JALAPENO")        newLevel->reward = JALAPENO;
    else if (input == "POTATOMINE")      newLevel->reward = POTATOMINE;
    else if (input == "ICESHROOM")       newLevel->reward = ICESHROOM;
    else if (input == "SQUASH")          newLevel->reward = SQUASH;
    else if (input == "PUFFSHROOM")      newLevel->reward = PUFFSHROOM;



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

    Array<bool> bigWaves;
    for (int i = 0; i < newLevel->numberOfWaves; i++) { // Big Waves
      file >> inputB;
      bigWaves.push(inputB);
    }

    for (int i = 0; i < newLevel->numberOfWaves; i++) {
      LevelManager::Level::Wave *newWave = new LevelManager::Level::Wave;
      newWave->delay = delays[i];
      newWave->duration = durations[i];
      newWave->isBigWave = bigWaves[i];
      newWave->waveSprite.setPosition({1100 - 145 * (delays[i]) / (delays[newLevel->numberOfWaves - 1] + durations[newLevel->numberOfWaves - 1]), 568});
      for (int z = 0; z < numberOfZombies[i]; z++) {
        file >> input; // zombie type

        if      (input == "Regular")    newWave->zombieTypes.push(Zombie::Type::Regular);
        else if (input == "Conehead")   newWave->zombieTypes.push(Zombie::Type::Conehead);
        else if (input == "Buckethead") newWave->zombieTypes.push(Zombie::Type::Buckethead);
        else if (input == "Flag")       newWave->zombieTypes.push(Zombie::Type::Flag);
        else if (input == "Screendoor") newWave->zombieTypes.push(Zombie::Type::Screendoor);
        else if (input == "Soccer")     newWave->zombieTypes.push(Zombie::Type::Soccer);
      }

      newLevel->waves.push(newWave);
    }

    levelManager.levels.push(newLevel);


    file.close();

  }
}



void updateFiles() {
  //std::cout << "Updating files..\n";

  std::ofstream file("storage/playerData.txt");
  if(file.is_open())
    file << maxLevelUnlocked;
  else
    std::cout << "Unable to open playerData.txt";

  file.close();

  // Write settings to file
  std::ofstream settingsFile("storage/settings.txt");
  if (!settingsFile.is_open()) {
    std::cout << "Unable to open settings.txt";
    return;
  }
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


  // ensure correct fullscreen state on startup
  if (settings.fullscreen) {
    window->create(sf::VideoMode::getDesktopMode(), "Plants vs Zombies", sf::Style::None, sf::State::Fullscreen);
    view->setViewport(sf::FloatRect({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
    dayLevel.camera.setViewport(sf::FloatRect({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
    gameView->setViewport(sf::FloatRect({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
  }
  else {
    window->create(sf::VideoMode(WINDOW_SIZE), "Plants vs Zombies"); // Default is windowed
    getLetterboxView(window->getSize().x, window->getSize().y);
  }
  setWindowMetaData();
}
