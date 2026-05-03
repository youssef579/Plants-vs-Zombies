#include <AssetsManager.hpp>
#include <Files.hpp>
#include <Game.hpp>
#include <Audio.hpp>
#include <SFML/Window/Mouse.hpp>
#include <UI/Overlay.hpp>
#include <Window.hpp>
#include <globals.hpp>
#include <ReAnimation/ReAnimation.hpp>

#include <PvP/Peer.hpp>

sf::Vector2f mousePosition;

Peer peer;

bool plants = 0;
unsigned short ports[2] = {53000, 53001};

int main() {
  initWindow();
  initFiles();
  initAssets();
  initAudio();
  initOverlay();
  initWeather();
  initReAnimDefs();
  pauseMenu.init();

  std::cin >> plants;

  if(plants == 0) peer.player = Peer::Plants;
  else peer.player = Peer::Zombies;

  peer.init(ports[plants]);

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
