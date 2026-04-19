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
#include <LevelManager.hpp>
#include <globals.hpp>
#include <Grid.hpp>

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

void updateGame() {
  dt = drawClock.restart() .asSeconds(); // clock.restart() sets time to 0 and returns the last
                         // time before modifying it
  // calling dt = clock.restart() each frame returns the time between frames
  // (dt)

  switch (gameState) {
  case 0:
    updateHome();
    break;
  default:
    //static Shovel shovel;
    if (runOnce) {
      //pauseMenu.init();
      shovel.init();
      initPackets();
      initGrid();
      dayLevel.init();
      gameWeather.isRaining = true;
      for (int i = 0; i < ROWS_NUMBER; i++){
        for (int j = 0; j < COLUMNS_NUMER; j++){
              // plants just for testing
              //grid[i][j].plant = Plant(WALLNUT, grid[i][j].plantPosition, 1, ReAnimator::getDefinition(REANIM_WALLNUT));
        }
      }
      music.play("DayStage");
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

    // These plants are for test only, gonna be removed in future
    static Plant s(SUN_FLOWER, {500 ,300}, 1, ReAnimator::getDefinition(REANIM_SUNFLOWER));
    static Plant t(WALLNUT, {300 ,200}, 1, ReAnimator::getDefinition(REANIM_WALLNUT));
    static Plant p(PEASHOOTER, {300 ,300}, 1, ReAnimator::getDefinition(REANIM_PEASHOOTER));
    static Plant q(SNOWPEASHOOTER, {300 ,400}, 1, ReAnimator::getDefinition(REANIM_SNOWPEA));
    static Plant v(REPEATERPEA, {300 ,500}, 1, ReAnimator::getDefinition(REANIM_REPEATER));


    //s.update(dt);
    //t.update(dt);
    //p.update(dt);
    //q.update(dt);
    //v.update(dt);
    updateGrid(dt);

    dayLevel.update(dt);
    dayLevel.draw(*window);
    window->setView(*view);
    //s.draw();
    //window->draw(rec1);
    //t.draw();
    //p.draw();
    //q.draw();
    //v.draw();
    drawGrid();

    
    for (int i = 0; i < bullets.size; i++) {
      bullets[i].update(dt);
      bullets[i].draw();
    }

    bullets.erase([](const Bullet& b) {
      return b.remove;
    });

    drawUI();
    shovel.drawBank();
    Sun::manageSuns(dt);

    //for (int i = 0; i < packets.size; i++) {
    //  packets[i].update(dt);
    //  //packets[i].draw();
    //}
    updateSeedPackets(dt);
    drawSeedPackets();
    

    shovel.update();

    //s.draw();
    //t.draw();
    //p.draw();
    //q.draw();
    //v.draw();
    drawGrid();

    for (int i = 0; i < Sun::sunArrayCntr; i++) {
      if (Sun::sunArray[i])
        Sun::sunArray[i]->draw();
    }
    shovel.drawMovingShovel();

    for (int i = 0; i < packets.size; i++)
      packets[i].drawSelectedPlant();
    gameWeather.update(dt);
    break;
  }
}