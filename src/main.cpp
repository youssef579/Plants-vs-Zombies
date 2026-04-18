#include <AssetsManager.hpp>
#include <Files.hpp>
#include <Game.hpp>
#include <Audio.hpp>
#include <SFML/Window/Mouse.hpp>
#include <UI/Overlay.hpp>
#include <Window.hpp>
#include <globals.hpp>
#include <ReAnimation/ReAnimation.hpp>

sf::Vector2f mousePosition;

int main() {
  initWindow();
  initFiles();
  initAssets();
  initAudio();
  initOverlay();
  initWeather();
  initReAnimDefs();

  while (window->isOpen()) {
    mousePosition = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
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
