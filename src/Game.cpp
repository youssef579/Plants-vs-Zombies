#include <Array.hpp>
#include <AssetsManager.hpp>
#include <Audio.hpp>
#include <BackgroundManager.hpp>
#include <Bullet.hpp>
#include <Game.hpp>
#include <Grid.hpp>
#include <Home.hpp>
#include <LawnMower.hpp>
#include <LevelManager.hpp>
#include <LevelProgress.hpp>
#include <Packets/Packet.hpp>
#include <Packets/Shovel.hpp>
#include <ParticleSystem.hpp>
#include <PlantSelector.hpp>
#include <Plants/Peashooter.hpp>
#include <Plants/Plant.hpp>
#include <Plants/Repeaterpea.hpp>
#include <Plants/SnowpeaShooter.hpp>
#include <Plants/SunFlower.hpp>
#include <Plants/Wallnut.hpp>
#include <Rewards.hpp>
#include <SunManager.hpp>
#include <UI/Overlay.hpp>
#include <UI/TransitionManager.hpp>
#include <Weather.hpp>
#include <Window.hpp>
#include <Zombies/Zombie.hpp>
#include <globals.hpp>
#include <newPauseMenu.hpp>

// bool isOpen = false;
int gameState = 0;
/*
  0 -> Home menu
*/
float globalTimeModifier = 1.0f;

Array<Bullet> bullets;

sf::Clock drawClock;
float dt; // Delta Time (time between each frame draw)

static bool weatherInited = false;

bool isPaused = false;
bool runOnce = true;

void updateGame() {
  dt = drawClock.restart() .asSeconds(); // clock.restart() sets time to 0 and returns the last
                         // time before modifying it
  // calling dt = clock.restart() each frame returns the time between frames
  // (dt)
  dt *= settings.timeModifier * globalTimeModifier;

  TransitionManager::update(dt);

  switch (gameState) {
  case 0:
    updateHome();
    break;
  default:
    if (runOnce) {
      shovel.init();
      Array<PlantType> plantTypes;
      // plantTypes.push(PEASHOOTER);
      // plantTypes.push(SUN_FLOWER);
      // plantTypes.push(WALLNUT);
      // plantTypes.push(TALLNUT);
      // plantTypes.push(REPEATERPEA);
      // plantTypes.push(SNOWPEASHOOTER);
      // plantTypes.push(CHERRYBOMB);
      // plantTypes.push(JALAPENO);
      // plantTypes.push(POTATOMINE);
      // plantTypes.push(ICESHROOM);
      // plantTypes.push(SQUASH);
      fillPackets(plantTypes);
      // initGrid();
      plantSelector.initSelector();
      dayLevel.init();
      newPause.init();
      gameWeather.isRaining = true;

      music.play("DayStage");

      Zombie::init();
      RewardManager::init();

      levelManager.loadUnlockedPlants();

      runOnce = false;

      // REMOVE LATER
      /*LawnMower::activateLawnMower(0);
      LawnMower::activateLawnMower(1);
      LawnMower::activateLawnMower(2);
      LawnMower::activateLawnMower(3);
      LawnMower::activateLawnMower(4);*/
    }

    if (newPause.isOpen) {
      newPause.update(*window);

      gameWeather.update(dt);

      if (dayLevel.dirtSound && static_cast<int>(dayLevel.dirtSound->getStatus()) == 2) {
        dayLevel.dirtSound->pause();
        dayLevel.dirtSoundStarted = false;
      }

      dayLevel.draw(*window);

      window->setView(*view);
      drawGrid();
      Zombie::drawAll();
      Bullet::drawAll();
      gameWeather.draw(*window);
      drawUI();
      drawSeedPackets();
      shovel.drawBank();
      Sun::drawAll();

      sf::View uiView = window->getView();

      newPause.draw(*window);

      window->setView(uiView);

      return;
    }
     if (isPaused) {
       if (dayLevel.dirtSound &&
           (int)(dayLevel.dirtSound->getStatus()) == 2) {
         dayLevel.dirtSound->pause();
         dayLevel.dirtSoundStarted = false;
       }
       dayLevel.draw(*window);
       window->setView(*view);
       gameWeather.draw(*window);
       pauseMenu.update();
       pauseMenu.draw();
       break;
     }

    levelManager.update(dt);
    updateGrid(dt);

    dayLevel.update(dt);
    plantSelector.updateSelector(dt, *window);

    dayLevel.draw(*window);
    window->setView(*view);
    drawGrid();

    Bullet::updateAll(dt);
    Zombie::updateAll(dt);

    Zombie::drawAll();
    Bullet::drawAll();
    RewardManager::update(dt);
    ReAnimator::updatePhysicsObjects(dt);
    ReAnimator::drawPhysicsObjects(window);

    drawUI(dt);
    shovel.drawBank();
    Sun::manageSuns(dt);

    updateSeedPackets(dt);
    drawSeedPackets();

    shovel.update();

    ParticleSystem::update(dt);
    ParticleSystem::draw();
    ReAnimator::updateOrphans(dt);
    ReAnimator::drawOrphans();

    LawnMower::updateAll(dt);

    Sun::drawAll();

    shovel.drawMovingShovel();
    drawTimeModifier(dt);

    for (int i = 0; i < packets.size; i++)
      packets[i].drawSelectedPlant();
    gameWeather.update(dt);

    dayLevel.drawOverlays(*window);
    RewardManager::draw();

    plantSelector.drawSelector(*window);
    break;
  }
  TransitionManager::draw();
}
