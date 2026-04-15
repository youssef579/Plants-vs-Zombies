#include <Window.hpp>
#include <globals.hpp>
#include <AssetsManager.hpp>
#include <Plants/Packet.hpp>
#include <SunManager.hpp>

Array<SeedPacket> packets;

void initPackets() {
  static sf::Texture& peashooterTexture = getTexture("assets/Plants/peashooter.png");
  static sf::Sprite peashooterSprite(peashooterTexture);
  peashooterSprite.setTextureRect({{0,0},{348,359}});
  peashooterSprite.setScale({0.225, 0.225});

  for (int i = 0; i < 7; i++)
    packets.push({100, 5, "peashooter", {90 + 59.0f * i, 11}, peashooterSprite});
}

SeedPacket::SeedPacket(int costValue, float reloadDurationValue, std::string packetName, sf::Vector2f position, sf::Sprite preview)
    : enabledSprite(getTexture("assets/packets/" + packetName + ".png")),
      disabledSprite(getTexture("assets/packets/" + packetName + "_disabled.png")),
      plantSprite(preview),
      cost(costValue),
      reloadDuration(reloadDurationValue) {
  float scaleFactor = 1.15;
  enabledSprite.setPosition(position);
  enabledSprite.setScale({scaleFactor, scaleFactor});

  disabledSprite.setPosition(position);
  disabledSprite.setScale({scaleFactor, scaleFactor});

  plantSprite.setOrigin(plantSprite.getLocalBounds().getCenter());
}

void SeedPacket::update(float dt) {
  reloadTimer = std::max(0.0f, reloadTimer - dt);

  sf::Vector2f mousePosition = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
  if (Sun::sunBalance >= cost && reloadTimer == 0 && isMousePressed && enabledSprite.getGlobalBounds().contains(mousePosition))
    selected = true;

  if (selected)
    plantSprite.setPosition(mousePosition);

  if (selected && isMouseReleased) {
    reloadTimer = reloadDuration;
    Sun::sunBalance -= cost;
    selected = false;
  }

  if (reloadTimer > 0)
    enabledSprite.setTextureRect({{0, 0}, {50, (int)(70 * (reloadDuration - reloadTimer) / reloadDuration)}});
}

void SeedPacket::draw() {
  window->draw(disabledSprite);

  if (Sun::sunBalance >= cost)
    window->draw(enabledSprite);

  if (selected)
    window->draw(plantSprite);
}

