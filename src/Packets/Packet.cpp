#include <Array.hpp>
#include <AssetsManager.hpp>
#include <Packets/Packet.hpp>
#include <Plants/Plant.hpp>
#include <SunManager.hpp>
#include <Window.hpp>
#include <algorithm>
#include <globals.hpp>

Array<SeedPacket> packets;

void fillPackets(Array<PlantType> &types) {
  static bool runOnce = true;
  static sf::Texture &peashooterTexture =
      getTexture("assets/Plants/peashooter.png");
  static sf::Sprite peashooterSprite(peashooterTexture);

  static sf::Texture &sunFlowerTexture =
      getTexture("assets/Plants/sunflower.png");
  static sf::Sprite sunFlowerSprite(sunFlowerTexture);

  static sf::Texture &wallNutTexture = getTexture("assets/Plants/wallnut.png");
  static sf::Sprite wallNutSprite(wallNutTexture);

  static sf::Texture &icepeaTexture = getTexture("assets/Plants/Icepea.png");
  static sf::Sprite icepeaSprite(icepeaTexture);

  static sf::Texture &repeaterpeaTexture =
      getTexture("assets/Plants/Repeaterpea.png");
  static sf::Sprite repeaterpeaSprite(repeaterpeaTexture);

  static sf::Texture &tallNutTexture =
      getTexture("assets/Plants/tallnut/Tallnut_body.png");
  static sf::Sprite tallNutSprite(tallNutTexture);

  static sf::Texture &cherryBombTexture =
      getTexture("assets/Plants/cherrybomb.png");
  static sf::Sprite cherryBombSprite(cherryBombTexture);

  static sf::Texture &jalapenoTexture =
      getTexture("assets/Plants/jalapeno.png");
  static sf::Sprite jalapenoSprite(jalapenoTexture);

  static sf::Texture &potatoMineTexture =
      getTexture("assets/Plants/potatoMine.png");
  static sf::Sprite potatoMineSprite(potatoMineTexture);

  if (runOnce) {
    peashooterSprite.setTextureRect({{0, 0}, {348, 359}});
    peashooterSprite.setScale({0.225, 0.225});

    sunFlowerSprite.setTextureRect({{0, 0}, {80, 80}});

    wallNutSprite.setTextureRect({{0, 0}, {65, 73}});

    icepeaSprite.setTextureRect({{0, 0}, {353, 368}});
    icepeaSprite.setScale({0.218, 0.217});

    repeaterpeaTexture.setSmooth(true);
    repeaterpeaSprite.setTextureRect({{0, 0}, {73, 71}});

    tallNutSprite.setScale({0.8f, 0.8f});

    // cherryBombSprite.setScale({ 0.8f, 0.8f });

    jalapenoSprite.setScale({0.8f, 0.8f});

    potatoMineSprite.setScale({0.135f, 0.135f});
    runOnce = false;
  }

  for (int i = 0; i < types.size; i++) {
    switch (types[i]) {
    case PEASHOOTER:
      packets.push({100,
                    5,
                    "peashooter",
                    {90 + 59.0f * i, 11},
                    peashooterSprite,
                    PEASHOOTER});
      break;

    case SUN_FLOWER:
      packets.push({50,
                    5,
                    "sunflower",
                    {90 + 59.0f * i, 11},
                    sunFlowerSprite,
                    SUN_FLOWER});
      break;

    case WALLNUT:
      packets.push(
          {50, 5, "wallnut", {90 + 59.0f * i, 11}, wallNutSprite, WALLNUT});
      break;

    case SNOWPEASHOOTER:
      packets.push({175,
                    5,
                    "peaice",
                    {90 + 59.0f * i, 11},
                    icepeaSprite,
                    SNOWPEASHOOTER});
      break;

    case REPEATERPEA:
      packets.push({200,
                    5,
                    "repeated",
                    {90 + 59.0f * i, 11},
                    repeaterpeaSprite,
                    REPEATERPEA});
      break;

    case TALLNUT:
      packets.push(
          {125, 5, "tallnut", {90 + 59.0f * i, 11}, tallNutSprite, TALLNUT});
      break;

    case CHERRYBOMB:
      packets.push({125,
                    1,
                    "cherrybomb",
                    {90 + 59.0f * i, 11},
                    cherryBombSprite,
                    CHERRYBOMB});
      break;

    case JALAPENO:
      packets.push(
          {125, 1, "jalapeno", {90 + 59.0f * i, 11}, jalapenoSprite, JALAPENO});
      break;

    case POTATOMINE:
      packets.push({25,
                    1,
                    "potatomine",
                    {90 + 59.0f * i, 11},
                    potatoMineSprite,
                    POTATOMINE});
      break;
    }
  }
}

SeedPacket::SeedPacket(int costValue, float reloadDurationValue,
                       std::string packetName, sf::Vector2f position,
                       sf::Sprite preview, PlantType plantTypeValue)
    : enabledSprite(getTexture("assets/packets/" + packetName + ".png")),
      disabledSprite(
          getTexture("assets/packets/" + packetName + "_disabled.png")),
      plantSprite(preview), plantShadow(preview), cost(costValue),
      plantType(plantTypeValue), selected(false), reloadTimer(0),
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

  if (Sun::sunBalance >= cost && reloadTimer == 0 && isMousePressed &&
      enabledSprite.getGlobalBounds().contains(mousePosition))
    selected = true;

  if (selected)
    plantSprite.setPosition(mousePosition);

  if (selected && isMouseReleased) {
    selected = false;
  }

  if (reloadTimer > 0)
    enabledSprite.setTextureRect(
        {{0, 0},
         {50, (int)(70 * (reloadDuration - reloadTimer) / reloadDuration)}});
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
