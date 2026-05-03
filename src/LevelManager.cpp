#include <LevelManager.hpp>
#include <BackgroundManager.hpp>
#include <Game.hpp>
#include <Bullet.hpp>
#include <Rewards.hpp>
#include <LawnMower.hpp>
#include <LevelProgress.hpp>

LevelManager levelManager;


void LevelManager::resetLevelData() { // reset all variables and timers for clean start on level

  // Background
  for (int i = 0; i < 5; i++) dayLevel.timers[i] = 0;
  for (int i = 0; i < 5; i++) dayLevel.isRolling[i] = 0;
  for (int i = 0; i < 5; i++) dayLevel.shouldStartRolling[i] = 0;
  for (int i = 0; i < 5; i++) dayLevel.timerFinished[i] = 0;
  for (int i = 0; i < 5; i++) dayLevel.capSprites[i] = nullptr;
  for (int i = 0; i < 5; i++) dayLevel.rollSprites[i] = nullptr;
  dayLevel.isIntroRunning = true;
  dayLevel.state = dayLevel.State::Game;
  dayLevel.isThreeMiddleRunning = false;
  dayLevel.isFullGrassRunning = false;
  dayLevel.isFullGrassFinished = false;
  dayLevel.isThreeMiddleFinished = false;
  dayLevel.dirtSoundStarted = false;
  dayLevel.fullGrassCurrentX = 0;
  dayLevel.threeMiddleCurrentX = 0;
  dayLevel.zombiesWonTimer = 0;
  dayLevel.gameOverTimer = 0;
  dayLevel.introTimer = -1;
  if(dayLevel.backGroundSprite) dayLevel.backGroundSprite->setOrigin({ 0, 0 });
  if(dayLevel.backGroundSprite) dayLevel.backGroundSprite->setScale({ 1, 1 });

  for (int i = 0; i < 5; i++) if(dayLevel.grassSprites[i]) dayLevel.grassSprites[i]->setOrigin({0.0f, 0.0f});
  if(dayLevel.threeMiddleGrassSprite) dayLevel.threeMiddleGrassSprite->setOrigin({ 0.0f, 0.0f });
  if(dayLevel.fullGrassSprite) dayLevel.fullGrassSprite->setOrigin({ 0.0f, 0.0f });
  globalTimeModifier = 1.0f;
  dayLevel.init();


  // Zombies
  for (int r = 0; r < ROWS_NUMBER; r++) zombies[r].erase([](Zombie *z) { return true; });
  Zombie::totalZombies = 0;
  // Bullets
  bullets.erase([](Bullet &b) {return true; });

  gameView->setCenter({0, 0});

  spawningFinished = false;
  Sun::isSpawning = true;
  Sun::spawnTimer = -20;

  // Rewards
  RewardManager::spawnedLevelReward = false;
  RewardManager::isPacketCollected = false;
  RewardManager::rewards.erase([](RewardManager::Reward &r) {return true; });

  resetLevelProgress();

  // Packets
  fillPackets(Array<PlantType>());
  //plantSelector.initSelector();
  dayLevel.playBtnClicked = false;
  dayLevel.introSlidingStarted = false;
  dayLevel.isWaitingForPlay = false;
  plantSelector.isVisible = false;
  plantSelector.tarX = -600.0f;
  plantSelector.currentSelectedCnt = 0;
  chosenPlants.erase([](PlantType &p) {return true; });


  for (int j = 0; j < packetsNum; j++) {
    plantSelector.packets[j].isSelected = false;
    plantSelector.packets[j].sprite->setColor(sf::Color::White);
  }

  for (int i = 0; i < 7; i++) {
    plantSelector.selectedSlot[i].active = false;
    if (plantSelector.selectedSlot[i].sprite) {
      delete plantSelector.selectedSlot[i].sprite;
      plantSelector.selectedSlot[i].sprite = nullptr;
    }
  }
  
  


  

}


void LevelManager::loadLevelData(int levelNum) {
  if (levelNum > ACTUAL_MAX_LEVELS) {
    std::cout << "Level " << levelNum << " not implemented yet!\nLoading latest Level..\n";
    levelNum--;
  }
  resetLevelData();
  currentLevel = levelNum;
  currentWave = 0;
  timer = -20;

  clearGrid();
  Sun::clear();

  for (int i = 0; i < 8; i++) { // spawn dummy zombies for intro
    int R = rand() % ROWS_NUMBER;
    float cent = grid[R][0].rectangle.getGlobalBounds().getCenter().y;
    Zombie::createZombie(randomRange(1250, 1250 + 150), randomRange(cent-30, cent+30), ((Zombie::Type)(rand() % 4)), R, 1000.0f);
  }

   LawnMower::init();
  //static bool runOnce = [](){
      //return true;
    //}();


}

void LevelManager::restartLevel() {
  resetLevelData();
  loadLevelData(currentLevel);
  isPaused = false;
  gameWeather.isPaused = false;
}

void LevelManager::update(float dt) {
  if(dayLevel.introTimer>5.5f)
    timer += dt;
  static int lastRow = 0;


  if (spawningFinished || currentWave >= levels[currentLevel - 1]->waves.size) { // Level Spawning finished
    Sun::isSpawning = false; // disable sun spawning
    return;
  }

  static LevelManager::Level::Wave *currWave = nullptr;
  currWave = levels[currentLevel - 1]->waves[currentWave];

  //std::cout << "Current Wave: " << currentWave << "\n";
  //std::cout << "Timer: " << timer << "\n";
  //std::cout << "waveDelay: " << currWave->delay << "\n";

  if (timer < currWave->delay) {
    return;
  }

  zombieSpawnDelay = currWave->duration / currWave->zombieTypes.size; // delay between each zombie in wave

  zombieSpawnTimer += dt;
  //std::cout << zombieSpawnTimer << "\n";
  if (zombieSpawnTimer >= zombieSpawnDelay) {
    int newRow = randomRange(0, ROWS_NUMBER-1);
    if (newRow == lastRow) newRow = randomRange(0, ROWS_NUMBER - 1);

    lastRow = newRow;

    /*std::cout << "Spawned zombie at row " << newRow << " of type ";
    switch (currWave->zombieTypes[zombiesSpawned]) {
    case Zombie::Regular:
      std::cout << "Regular\n";
      break;
    case Zombie::Conehead:
      std::cout << "Conehead\n";
      break;
    case Zombie::Buckethead:
      std::cout << "Buckethead\n";
      break;
    case Zombie::Flag:
      std::cout << "Flag\n";
      break;
    case Zombie::Screendoor:
      std::cout << "Screendoor\n";
      break;
    }*/

    //std::cout << "Spawned zombie------------------------------------------\n";
    Zombie::createZombie(1250, grid[newRow][COLUMNS_NUMBER-1].rectangle.getGlobalBounds().getCenter().y, currWave->zombieTypes[zombiesSpawned], newRow, 0);
    //zombieSpawnTimer -= zombieSpawnDelay;
    zombieSpawnTimer = 0;
    zombiesSpawned++;
    Zombie::totalZombies++;
  }

  if (zombiesSpawned == currWave->zombieTypes.size) {
    currentWave++;
    //zombieSpawnTimer = 0;
    zombiesSpawned = 0;
    sounds.play("Awooga");
    //std::cout << zombieSpawnTimer << "\n";
  }

  if (currentWave == levels[currentLevel - 1]->waves.size)
    spawningFinished = true;
  else
    zombieSpawnTimer = 999999.0f; // spawn first zombie instantly except for first wave



}
