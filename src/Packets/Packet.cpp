#include <Window.hpp>
#include <algorithm>
#include <globals.hpp>
#include <AssetsManager.hpp>
#include <Plants/Plant.hpp>
#include <Packets/Packet.hpp>
#include <SunManager.hpp>

Array<SeedPacket> packets;

void initPackets() {
  sf::Texture& peashooterTexture = getTexture("assets/Plants/peashooter.png");
  sf::Sprite peashooterSprite(peashooterTexture);
  peashooterSprite.setTextureRect({{0,0},{348,359}});
  peashooterSprite.setScale({0.225, 0.225});
  packets.push({100, 5, "peashooter", {90 + 59.0f * 0, 11}, peashooterSprite, PEASHOOTER});

  sf::Texture& sunFlowerTexture = getTexture("assets/Plants/sunflower.png");
  sf::Sprite sunFlowerSprite(sunFlowerTexture);
  sunFlowerSprite.setTextureRect({{0, 0}, {80, 80}});
  packets.push({50, 5, "sunflower", {90 + 59.0f * 1, 11}, sunFlowerSprite, SUN_FLOWER});

  sf::Texture& wallNutTexture = getTexture("assets/Plants/wallnut.png");
  sf::Sprite wallNutSprite(wallNutTexture);
  wallNutSprite.setTextureRect({{0, 0}, {65, 73}});
  packets.push({50, 5, "wallnut", {90 + 59.0f * 2, 11}, wallNutSprite, WALLNUT});

  sf::Texture& icepeaTexture = getTexture("assets/Plants/Icepea.png");
  sf::Sprite icepeaSprite(icepeaTexture);
  icepeaSprite.setTextureRect({ {0,0},{353, 368}});
  icepeaSprite.setScale({0.218, 0.217});
  packets.push({175, 5, "peaice", {90 + 59.0f * 3, 11}, icepeaSprite, SNOWPEASHOOTER});

  sf::Texture& repeaterpeaTexture = getTexture("assets/Plants/Repeaterpea.png");
  repeaterpeaTexture.setSmooth(true);
  sf::Sprite repeaterpeaSprite(repeaterpeaTexture);
  repeaterpeaSprite.setTextureRect({ {0,0},{73, 71}});
  packets.push({200, 5, "repeated", {90 + 59.0f * 4, 11}, repeaterpeaSprite, REPEATERPEA});

  sf::Texture &tallNutTexture = getTexture("assets/Plants/tallnut/Tallnut_body.png");
  sf::Sprite tallNutSprite(tallNutTexture);
  tallNutSprite.setScale({0.8f, 0.8f});
  packets.push({ 125, 5, "tallnut", {90 + 59.0f * 5, 11}, tallNutSprite, TALLNUT });

  sf::Texture &cherryBombTexture = getTexture("assets/Plants/cherrybomb.png");
  sf::Sprite cherryBombSprite(cherryBombTexture);
  //cherryBombSprite.setScale({ 0.8f, 0.8f });
  packets.push({ 125, 1, "cherrybomb", {90 + 59.0f * 6, 11}, cherryBombSprite, CHERRYBOMB });


  sf::Texture &jalapenoTexture = getTexture("assets/Plants/jalapeno.png");
  sf::Sprite jalapenoSprite(jalapenoTexture);
  jalapenoSprite.setScale({ 0.8f, 0.8f });
  packets.push({ 125, 1, "jalapeno", {90 + 59.0f * 7, 11}, jalapenoSprite, JALAPENO });

  sf::Texture &potatoMineTexture = getTexture("assets/Plants/potatoMine.png");
  sf::Sprite potatoMineSprite(potatoMineTexture);
  potatoMineSprite.setScale({ 0.135f, 0.135f });
  packets.push({ 25, 1, "potatomine", {90 + 59.0f * 8, 11}, potatoMineSprite, POTATOMINE });

}

SeedPacket::SeedPacket(int costValue, float reloadDurationValue, std::string packetName, sf::Vector2f position, sf::Sprite preview, PlantType plantTypeValue)
    : enabledSprite(getTexture("assets/packets/" + packetName + ".png")),
      disabledSprite(getTexture("assets/packets/" + packetName + "_disabled.png")),
      plantSprite(preview),
      plantShadow(preview),
      cost(costValue),
      plantType(plantTypeValue),
      selected(false),
      reloadTimer(0),
      reloadDuration(reloadDurationValue) {
  float scaleFactor = 1.15;
  enabledSprite.setPosition(position);
  enabledSprite.setScale({scaleFactor, scaleFactor});

  disabledSprite.setPosition(position);
  disabledSprite.setScale({scaleFactor, scaleFactor});

  plantSprite.setOrigin(plantSprite.getLocalBounds().getCenter());

  plantShadow.setOrigin(plantShadow.getLocalBounds().size / 2.0f);
  plantShadow.setColor(sf::Color{255, 255, 255, 100});
}

void SeedPacket::update(float dt) {
  reloadTimer = std::max(0.0f, reloadTimer - dt);

  if (Sun::sunBalance >= cost && reloadTimer == 0 && isMousePressed && enabledSprite.getGlobalBounds().contains(mousePosition))
    selected = true;

  if (selected)
    plantSprite.setPosition(mousePosition);

  if (selected && isMouseReleased) {
    selected = false;
  }

  if (reloadTimer > 0)
    enabledSprite.setTextureRect({{0, 0}, {50, (int)(70 * (reloadDuration - reloadTimer) / reloadDuration)}});
}

void SeedPacket::draw() {
  window->draw(disabledSprite);

  if (Sun::sunBalance >= cost)
    window->draw(enabledSprite);
}

void SeedPacket::drawSelectedPlant() {
  if (selected)
    window->draw(plantSprite);
}


void updateSeedPackets(float dt) {
  for (int i = 0; i < packets.size; i++) {
    packets[i].update(dt);
  }
}

void drawSeedPackets() {
  for (int i = 0; i < packets.size; i++) {
    packets[i].draw();
  }
}
