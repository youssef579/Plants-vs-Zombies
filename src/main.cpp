#include <AssetsManager.hpp>
#include <Files.hpp>
#include <Game.hpp>
#include <Audio.hpp>
#include <UI/Overlay.hpp>
#include <Window.hpp>
#include <globals.hpp>

int main() {
  initWindow();
  initFiles();
  initAssets();
  initAudio();
  initOverlay();
  initWeather();

  while (window->isOpen()) {
    handleEvents();

    window->clear();
    window->setView(*view);
    updateGame();
    window->display();
  }

  updateFiles();
  delete window;
  delete view;
  delete assets;
  delete overlay;
}
