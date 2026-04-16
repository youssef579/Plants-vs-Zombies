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
    if (runOnce) {
      pauseMenu.init();
      gameWeather.isRaining = true;

      Bullet b1{PEA, {300, 300}, 0};
      Bullet b2{SNOWPEA, {300, 400}, 0};
      bullets.push(b1);
      bullets.push(b2);
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

    s.draw();
    t.draw();
    p.draw();
    q.draw();
    v.draw();

    for (int i = 0; i < bullets.size; i++) {
      bullets[i].update(dt);
      bullets[i].draw();
    }

    bullets.erase([](const Bullet& b) {
      return b.remove;
    });

    Sun::manageSuns(dt);
    gameWeather.update(dt);
    drawUI();
    break;
  }
}


