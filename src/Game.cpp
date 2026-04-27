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
      plantTypes.push(SNOWPEASHOOTER);
      plantTypes.push(CHERRYBOMB);
      plantTypes.push(JALAPENO);
      plantTypes.push(POTATOMINE);
      fillPackets(plantTypes);
      //initGrid();
      dayLevel.init();
      gameWeather.isRaining = true;
      for (int i = 0; i < ROWS_NUMBER; i++){
        for (int j = 0; j < COLUMNS_NUMBER; j++){
              // plants just for testing
              //grid[i][j].plant = Plant(WALLNUT, grid[i][j].plantPosition, 1, ReAnimator::getDefinition(REANIM_WALLNUT));
        }
      }
      music.play("DayStage");

      Zombie::init();
      LawnMower::init();
      RewardManager::init();

      ////Zombie Testing
      //Zombie::createZombie(
      //  grid[2][8].rectangle.getGlobalBounds().getCenter().x + 300,
      //  grid[2][8].rectangle.getGlobalBounds().getCenter().y,
      //  Zombie::Type::Regular, 2, 0);
      //Zombie::createZombie(
      //  grid[2][8].rectangle.getGlobalBounds().getCenter().x + 400,
      //  grid[2][8].rectangle.getGlobalBounds().getCenter().y,
      //  Zombie::Type::Regular, 2, 0);
      //Zombie::createZombie(
      //  grid[1][8].rectangle.getGlobalBounds().getCenter().x + 300,
      //  grid[1][8].rectangle.getGlobalBounds().getCenter().y,
      //  Zombie::Type::Conehead, 1, 0);
      //Zombie::createZombie(
      //  grid[0][8].rectangle.getGlobalBounds().getCenter().x + 300,
      //  grid[0][8].rectangle.getGlobalBounds().getCenter().y,
      //  Zombie::Type::Buckethead, 0, 0);
      //Zombie::createZombie(
      //  grid[3][8].rectangle.getGlobalBounds().getCenter().x + 300,
      //  grid[3][8].rectangle.getGlobalBounds().getCenter().y,
      //  Zombie::Type::Flag, 3, 0);
      //Zombie::createZombie(
      //  grid[4][8].rectangle.getGlobalBounds().getCenter().x + 300,
      //  grid[4][8].rectangle.getGlobalBounds().getCenter().y,
      //  Zombie::Type::Screendoor, 4, 0);
      //zombies[4][0].startDelay = 4.0f;

      // REMOVE LATER
      /*LawnMower::activateLawnMower(0);
      LawnMower::activateLawnMower(1);
      LawnMower::activateLawnMower(2);
      LawnMower::activateLawnMower(3);
      LawnMower::activateLawnMower(4);*/




      runOnce = false;

    }

    if (isPaused) {
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



    //std::cout << "FlagPos: [" << z4.gridPosition.x << "][" << z4.gridPosition.y << "]\n";
    /*if (z4.health > 0)
      z4.takeDamage(0.2);
    if (z3.health > 0)
      z3.takeDamage(0.2);
    if (z2.health > 0)
      z2.takeDamage(0.2);
    if (z1.health > 0)
      z1.takeDamage(0.2);*/
    //static sf::Clock tmpC;
    //static float tmp = 0;
    //tmp = tmpC.getElapsedTime().asSeconds();
    ////std::cout << "state: " << z1.state << "\n";
    //if (tmp >= 2 && tmp <= 5) {
    //  //tmp = 0;
    //  z1.state = Zombie::State::Attacking;
    //  //tmpC.reset();
    //}
    //else if (tmp >= 5 && tmp <= 8) {
    //  z1.state = Zombie::State::Walking;
    //}
    //else if (tmp >= 8) {
    //  if(z1.health > 0)
    //    z1.takeDamage(10);
    //}


    levelManager.update(dt);
    updateGrid(dt);

    dayLevel.update(dt);
    dayLevel.draw(*window);
    window->setView(*view);
    drawGrid();

    /*for (int i = 0; i < bullets.size; i++) {
      bullets[i].update(dt);
      bullets[i].draw();
    }*/
    Bullet::updateAll(dt);
    Bullet::drawAll();


    Zombie::updateAll(dt);
    Zombie::drawAll();
    RewardManager::update(dt);

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
}
