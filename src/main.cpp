#include <AssetsManager.hpp>
#include <Audio.hpp>
#include <Files.hpp>
#include <Game.hpp>
#include <Overlay.hpp>
#include <Window.hpp>
#include <globals.hpp>

float deltaTime;

int main() {
  initWindow();
  initAssets();
  initFiles();
  initAudio();
  initOverlay();
  initWeather();

  // const auto onClose = [](const sf::Event::Closed &) { window->close(); };

  sf::Clock clock;
  while (window->isOpen()) {
    deltaTime =
        clock.restart()
            .asSeconds(); // clock.restart() sets time to 0 and returns the last
    // window->handleEvents(onClose);  //doesn't allow for key presses
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
