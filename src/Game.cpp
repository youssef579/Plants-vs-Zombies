#include "Plants/Plant.hpp"
#include <Packets/Shovel.hpp>
#include <Packets/Packet.hpp>
#include <AssetsManager.hpp>
#include <Audio.hpp>
#include <Game.hpp>
#include <Home.hpp>
#include <UI/Overlay.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <Window.hpp>
#include <globals.hpp>
#include <Weather.hpp>
#include <SunManager.hpp>
#include <Plants/SunFlower.hpp>
#include <Plants/Wallnut.hpp>
#include <Plants/Peashooter.hpp>
#include <Plants/SnowpeaShooter.hpp>
#include <Plants/Repeaterpea.hpp>
#include <SunManager.hpp>
#include <Array.hpp>
#include <Bullet.hpp>
#include <BackgroundManager.hpp>
#include <globals.hpp>
#include <Grid.hpp>
#include <Zombies/Zombie.hpp>
#include <ParticleSystem.hpp>
#include <LawnMower.hpp>
#include <LevelManager.hpp>
#include <Rewards.hpp>
#include <newPauseMenu.hpp>
#include <UI/TransitionManager.hpp>

//bool isOpen = false;
int gameState = 0;
/*
  0 -> Home menu
*/
float globalTimeModifier = 1.0f;

Array<Bullet>bullets;

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


  static bool pPressed = false;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)) {
    if (!pPressed) {
      newPause.isOpen = !newPause.isOpen;
      pPressed = true;
    }
  }
  else {
    pPressed = false;
  }
  if (newPause.isOpen) {
    newPause.update(*window);


    dayLevel.draw(*window);
    window->setView(*view);
    gameWeather.draw(*window);
    drawUI();
    drawSeedPackets();


    newPause.draw(*window);

    return;
  }

  TransitionManager::update(dt);

  switch (gameState) {
  case 0:
    updateHome();
    break;
  default:
    if (runOnce) {
      shovel.init();
      Array<PlantType> plantTypes;
      plantTypes.push(PEASHOOTER);
      plantTypes.push(SUN_FLOWER);
      plantTypes.push(WALLNUT);
      plantTypes.push(TALLNUT);
      plantTypes.push(REPEATERPEA);
      plantTypes.push(SNOWPEASHOOTER);
      plantTypes.push(CHERRYBOMB);
      plantTypes.push(JALAPENO);
      plantTypes.push(POTATOMINE);
      plantTypes.push(ICESHROOM);
      plantTypes.push(SQUASH);
      fillPackets(plantTypes);
      //initGrid();
      dayLevel.init();
      newPause.init();
      gameWeather.isRaining = true;

      music.play("DayStage");

      Zombie::init();
      RewardManager::init();

      runOnce = false;

      // REMOVE LATER
      /*LawnMower::activateLawnMower(0);
      LawnMower::activateLawnMower(1);
      LawnMower::activateLawnMower(2);
      LawnMower::activateLawnMower(3);
      LawnMower::activateLawnMower(4);*/

    }

    if (isPaused || newPause.isOpen) {
      if (dayLevel.dirtSound && static_cast<int>(dayLevel.dirtSound->getStatus()) == 2) {
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
    dayLevel.draw(*window);
    window->setView(*view);
    drawGrid();

    Bullet::updateAll(dt);
    Bullet::drawAll();


    Zombie::updateAll(dt);
    Zombie::drawAll();
    RewardManager::update(dt);
    ReAnimator::updatePhysicsObjects(dt);
    ReAnimator::drawPhysicsObjects(window);

    drawUI();
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


    //drawGrid();

    Sun::drawAll();



    shovel.drawMovingShovel();
    drawTimeModifier(dt);

    for (int i = 0; i < packets.size; i++)
      packets[i].drawSelectedPlant();
    gameWeather.update(dt);


    dayLevel.drawOverlays(*window);
    RewardManager::draw();


    break;
  }
  TransitionManager::draw();


  // EE
  /*onClick(sf::FloatRect{ {963, 484}, {5, 5} }, 0, []() {
    static sf::Texture M3ZA_T = getTexture("assets/SelectorScreen/M3ZA.png");
    static sf::Sprite M3ZA(M3ZA_T);
    window->draw(M3ZA);
    }, []() {});*/
}
