#pragma once
#include <AssetsManager.hpp>
#include <Array.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <Zombies/Zombie.hpp>


struct LevelManager {
  int currentLevel   = 0; // 0 -> no level
  int currentWave    = 0;
  int zombiesSpawned = 0;
  float timer = 0;
  float zombieSpawnDelay = 0;
  float zombieSpawnTimer = 0;
  bool spawningFinished = false;

  struct Level {
    enum Location {Day, Night, Pool, Roof};
    struct Wave {
      float delay;
      float duration;
      Array<Zombie::Type> zombieTypes;
      sf::Sprite waveSprite = sf::Sprite(getTexture("assets/levelProgress/wave.png")); // Used in Level Progress
      bool isBigWave;
    };

    Location location = Day;
    int numberOfWaves = 0;
    Array<Wave*> waves;
    PlantType reward;
  };

  Array<Level*> levels;


  void resetLevelData();
  void loadLevelData(int levelNum);
  void restartLevel();
  void update(float dt);
  void startLevel();
  void loadUnlockedPlants();



};

extern LevelManager levelManager;
