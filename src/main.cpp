#include <AssetsManager.hpp>
#include <Files.hpp>
#include <Game.hpp>
#include <Audio.hpp>
#include <SFML/Window/Mouse.hpp>
#include <UI/Overlay.hpp>
#include <Window.hpp>
#include <globals.hpp>
#include <ReAnimation/ReAnimation.hpp>
#include <ParticleSystem.hpp>
#include <Grid.hpp>
#include <PlantSelector.hpp>

sf::Vector2f mousePosition;

//std::vector<sf::FloatRect> GLOB_RECTS = {};

int main() {
  initWindow();
  initAssets();
  initFiles();
  initAudio();
  initOverlay();
  initWeather();
  initReAnimDefs();
  ParticleSystem::init();
  pauseMenu.init();
  initGrid();
  initGameProgress();
  plantSelector.initSelector();
  setWindowMetaData();

  while (window->isOpen()) {
    mousePosition = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
    handleEvents();

    
    window->clear();
    window->setView(*view);
    updateGame();

    /*for (auto GLOB_RECT : GLOB_RECTS) {
      sf::RectangleShape rect(GLOB_RECT.size);
      rect.setPosition(GLOB_RECT.position);
      rect.setFillColor(sf::Color(255, 0, 0, 100));
      window->draw(rect);
    }
    GLOB_RECTS.clear();*/

    window->display();


  }

  updateFiles();
  delete window;
  delete view;
  delete assets;
  delete overlay;
}
