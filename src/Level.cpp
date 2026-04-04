#include <Audio.hpp>
#include <Level.hpp>
#include <SunManager.hpp>
#include <UI/Draw.hpp>
#include <UI/Pause.hpp>

void updateLevel() {
  static bool runOnce = true;

  if (runOnce) {
    gameWeather.isRaining = true;

    playMusic("DayStage");
    runOnce = false;
  }

  if (!isPaused) {
    manageSuns();
    gameWeather.update();
  }

  drawUI();
  if (isPaused)
    updatePause();
}
