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
#include <globals.hpp>

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
    static Shovel shovel;
    if (runOnce) {
      pauseMenu.init();
      initPackets();
      gameWeather.isRaining = true;

      music.play("DayStage");
      runOnce = false;
    }

    if (isPaused) {
      pauseMenu.update();
      pauseMenu.draw();
      break;
    }

    // These plants are for test only, gonna be removed in future
    static Plant s(SUN_FLOWER, {300 ,100}, 1, getTexture("assets/Plants/sunflower.png"), 80, 80, 24, {1, 1});
    static Plant t(WALLNUT, {300 ,200}, 1, getTexture("assets/Plants/wallnut.png"), 65, 73, 16, {1, 1});
    static Plant p(PEASHOOTER, {300 ,300}, 1, getTexture("assets/Plants/peashooter.png"), 348, 359, 29, {0.225, 0.225});
    static Plant q(SNOWPEASHOOTER, {300 ,400}, 1, getTexture("assets/Plants/Icepea.png"), 353, 368, 25, {0.218, 0.217});
    static Plant v(REPEATERPEA, {300 ,500}, 1, getTexture("assets/Plants/Repeaterpea.png"), 73, 71, 15, {1, 1});

    s.update(dt);
    t.update(dt);
    p.update(dt);
    q.update(dt);
    v.update(dt);

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

    for (int i = 0; i < packets.size; i++) {
      packets[i].update(dt);
      packets[i].draw();
    }
    shovel.update();

    s.draw();
    t.draw();
    p.draw();
    q.draw();
    v.draw();

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


