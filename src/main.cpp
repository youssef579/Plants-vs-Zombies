#include <AssetsManager.hpp>
#include <Game.hpp>
#include <Overlay.hpp>
#include <SFML/Graphics.hpp>
#include <Window.hpp>
#include <globals.hpp>

int main() {
  initWindow();
  initAssets();
  initOverlay();

  const auto onClose = [&](const sf::Event::Closed &) { window->close(); };
  while (window->isOpen()) {
    window->handleEvents(onClose);

    window->clear();
    updateGame();
    window->display();
  }

  delete window;
  delete assets;
  delete overlay;
}
