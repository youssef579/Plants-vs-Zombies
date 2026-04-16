#include <Window.hpp>
#include <globals.hpp>
#include <Packets/Shovel.hpp>
#include <AssetsManager.hpp>

Shovel::Shovel()
  : shovelBank(getTexture("assets/packets/shovel_bank.png")),
    shovel(getTexture("assets/packets/shovel.png")),
    movingShovel(getTexture("assets/packets/shovel.png")) {
  shovelBank.setPosition({ 513, 0 });

  shovel.setPosition({520, 3});
  shovel.setScale({ 0.5, 0.5 });

  movingShovel.setOrigin(movingShovel.getLocalBounds().getCenter());
  movingShovel.setScale({0.6, 0.6});
}

void Shovel::update() {
  if (isMousePressed && shovelBank.getGlobalBounds().contains(mousePosition))
    selected = true;

  if (selected)
    movingShovel.setPosition(mousePosition);

  if (selected && isMouseReleased)
    selected = false;
}

void Shovel::draw() {
  window->draw(shovelBank);
  window->draw(shovel);

  if (selected)
    window->draw(movingShovel);
}

