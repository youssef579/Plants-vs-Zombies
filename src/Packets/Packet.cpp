#include <Window.hpp>
#include <globals.hpp>
#include <AssetsManager.hpp>
#include <Packets/Packet.hpp>
#include <SunManager.hpp>

Array<SeedPacket> packets;

void initPackets() {
  sf::Texture& peashooterTexture = getTexture("assets/Plants/peashooter.png");
  sf::Sprite peashooterSprite(peashooterTexture);
  peashooterSprite.setTextureRect({{0,0},{348,359}});
  peashooterSprite.setScale({0.225, 0.225});
  packets.push({100, 5, "peashooter", {90 + 59.0f * 0, 11}, peashooterSprite});

  sf::Texture& sunFlowerTexture = getTexture("assets/Plants/sunflower.png");
  sf::Sprite sunFlowerSprite(sunFlowerTexture);
  sunFlowerSprite.setTextureRect({{0, 0}, {80, 80}});
  packets.push({50, 5, "sunflower", {90 + 59.0f * 1, 11}, sunFlowerSprite});

  sf::Texture& wallNutTexture = getTexture("assets/Plants/wallnut.png");
  sf::Sprite wallNutSprite(wallNutTexture);
  wallNutSprite.setTextureRect({{0, 0}, {65, 73}});
  packets.push({50, 5, "wallnut", {90 + 59.0f * 2, 11}, wallNutSprite});

  sf::Texture& icepeaTexture = getTexture("assets/Plants/Icepea.png");
  sf::Sprite icepeaSprite(icepeaTexture);
  icepeaSprite.setTextureRect({ {0,0},{353, 368}});
  icepeaSprite.setScale({0.218, 0.217});
  packets.push({175, 5, "peaice", {90 + 59.0f * 3, 11}, icepeaSprite});

  sf::Texture& repeaterpeaTexture = getTexture("assets/Plants/Repeaterpea.png");
  repeaterpeaTexture.setSmooth(true);
  sf::Sprite repeaterpeaSprite(repeaterpeaTexture);
  repeaterpeaSprite.setTextureRect({ {0,0},{73, 71}});
  packets.push({200, 5, "repeated", {90 + 59.0f * 4, 11}, repeaterpeaSprite});
}

SeedPacket::SeedPacket(int costValue, float reloadDurationValue, std::string packetName, sf::Vector2f position, sf::Sprite preview)
    : enabledSprite(getTexture("assets/packets/" + packetName + ".png")),
      disabledSprite(getTexture("assets/packets/" + packetName + "_disabled.png")),
      plantSprite(preview),
      cost(costValue),
      selected(false),
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

