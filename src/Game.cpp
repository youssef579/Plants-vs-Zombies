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

int gameState = 0;
/*
  0 -> Home menu
*/

sf::Clock drawClock;
float dt; // Delta Time (time between each frame draw)

static bool weatherInited = false;

bool isPaused = false;
bool runOnce = true;

void updateGame() {
  dt = drawClock.restart()
           .asSeconds(); // clock.restart() sets time to 0 and returns the last
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

      music.play("DayStage");
      runOnce = false;
    }

    if (isPaused) {
      pauseMenu.update();
      pauseMenu.draw();
      break;
    }


    
    Sun::manageSuns(dt);
    gameWeather.update(dt);
    drawUI();
    break;
  }
}


