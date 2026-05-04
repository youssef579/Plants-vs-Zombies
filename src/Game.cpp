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

#include <PvP/Peer.hpp>

int gameState = 0;
/*
  0 -> Home menu
*/

Array<Bullet>bullets;

sf::Clock drawClock;
float dt; // Delta Time (time between each frame draw)

static bool weatherInited = false;

bool isPaused = false;
bool runOnce = true;

Peer::Command command = Peer::Heartbeat;
int row = -1, column = -1, type = -1;

void updateGame() {
  dt = drawClock.restart() .asSeconds(); // clock.restart() sets time to 0 and returns the last
                         // time before modifying it
  // calling dt = clock.restart() each frame returns the time between frames
  // (dt)

  dt = 1.f / 60;

  dt *= settings.timeModifier;

  switch (gameState) {
  case 0:
    updateHome();
    break;
  case 2:
    if(peer.state == Peer::InGame) gameState = 1;
    if(peer.state == Peer::Requesting || peer.state == Peer::Accepting) peer.connect();
    peer.receive();
    // std::cout << peer.nextTick << '\n';
    updateHome();
    break;
  default:
    if (runOnce) {
      shovel.init();
      initPackets();
      initGrid();
      dayLevel.init();
      gameWeather.isRaining = true;
      for (int i = 0; i < ROWS_NUMBER; i++){
        for (int j = 0; j < COLUMNS_NUMBER; j++){
              // plants just for testing
              //grid[i][j].plant = Plant(WALLNUT, grid[i][j].plantPosition, 1, ReAnimator::getDefinition(REANIM_WALLNUT));
        }
      }
      music.play("DayStage");

      //Zombie Testing
      // Zombie::createZombie(
      //   grid[2][8].rectangle.getGlobalBounds().getCenter().x + 300,
      //   grid[2][8].rectangle.getGlobalBounds().getCenter().y,
      //   Zombie::Type::Regular, 2);
      Zombie::createZombie(
        grid[1][8].rectangle.getGlobalBounds().getCenter().x + 300,
        grid[1][8].rectangle.getGlobalBounds().getCenter().y,
        Zombie::Type::Conehead, 1);
      // Zombie::createZombie(
      //   grid[0][8].rectangle.getGlobalBounds().getCenter().x + 300,
      //   grid[0][8].rectangle.getGlobalBounds().getCenter().y,
      //   Zombie::Type::Buckethead, 0);
      Zombie::createZombie(
        grid[3][8].rectangle.getGlobalBounds().getCenter().x + 300,
        grid[3][8].rectangle.getGlobalBounds().getCenter().y,
        Zombie::Type::Flag, 3);


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

    auto packet = Peer::createPacket(peer.nextTick, command, row, column, type);
    peer.send(packet);

    command = Peer::Heartbeat;
    // std::cout << "Sending " << peer.nextTick << '\n';

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
    
    

    updateGrid(dt);

    if(dt)dayLevel.update(dt);
    dayLevel.draw(*window);
    window->setView(*view);
    drawGrid();
    
    for (int i = 0; i < bullets.size; i++) {
      if(dt) bullets[i].update(dt);
      bullets[i].draw();
    }

    bullets.erase([](const Bullet& b) {
      return b.remove;
    });

    Zombie::updateAll(dt);
    Zombie::drawAll(dt);

    drawUI();
    if(peer.player == Peer::Plants) shovel.drawBank();
    Sun::manageSuns(dt);

    updateSeedPackets(dt);
    drawSeedPackets();

    shovel.update();



    //drawGrid();

    Sun::drawAll();



    shovel.drawMovingShovel();
    if(dt) drawTimeModifier(dt);

    for (int i = 0; i < packets.size; i++)
      packets[i].drawSelectedPlant();
    
    for(int i = 0; i < zombiePackets.size; i++)
      zombiePackets[i].drawSelectedPlant();

    if(dt) gameWeather.update(dt);

    if(peer.state == Peer::InGame) settings.timeModifier = 0;
    peer.receive();
    break;
  }
}
