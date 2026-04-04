#include <AssetsManager.hpp>
#include <Files.hpp>
#include <Game.hpp>
#include <Audio.hpp>
#include <Overlay.hpp>
#include <Window.hpp>
#include <globals.hpp>

int main() {
  initWindow();
  initAssets();
  initFiles();
  initAudio();
  initOverlay();
  initWeather();

  //const auto onClose = [](const sf::Event::Closed &) { window->close(); };
  while (window->isOpen()) {
    //window->handleEvents(onClose);  //doesn't allow for key presses
    handleEvents();

    window->clear();
    updateGame();
    window->display();
  }

  updateFiles();
  delete window;
  delete assets;
  delete overlay;
}
