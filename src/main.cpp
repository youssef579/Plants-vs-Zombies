#include <AssetsManager.hpp>
#include <Game.hpp>
#include <Overlay.hpp>
#include <globals.hpp>
#include <files.hpp>
#include <Window.hpp>

sf::RenderWindow *window;

bool buttonWasClicked = false;

int main() {
  initWindow();
  initAssets();
  initFiles();
  initOverlay();

  const auto onClose = [](const sf::Event::Closed &) { window->close(); };
  while (window->isOpen()) {
    window->handleEvents(onClose);

    window->clear();
    updateGame();
    window->display();
  }

  updateFiles();
  delete window;
  delete assets;
  delete overlay;
}
