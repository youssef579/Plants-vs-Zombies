#include <BackgroundManager.hpp>
#include <Array.hpp>
#include <AssetsManager.hpp>
#include <Packets/Packet.hpp>
#include <Plants/Plant.hpp>
#include <SunManager.hpp>
#include <Window.hpp>
#include <algorithm>
#include <globals.hpp>
#include <BackgroundManager.hpp>
#include <PvP/Peer.hpp>

Array<SeedPacket> packets;
Array<SeedPacket> zombiePackets;

void fillPackets(Array<PlantType> &types) {

  static bool runOnce = true;
  static sf::Texture &peashooterTexture = getTexture("assets/Plants/peashooter.png");
  static sf::Sprite peashooterSprite(peashooterTexture);

  static sf::Texture &sunFlowerTexture = getTexture("assets/Plants/sunflower.png");
  static sf::Sprite sunFlowerSprite(sunFlowerTexture);

  static sf::Texture &wallNutTexture = getTexture("assets/Plants/wallnut.png");
  static sf::Sprite wallNutSprite(wallNutTexture);

  static sf::Texture &icepeaTexture = getTexture("assets/Plants/Icepea.png");
  static sf::Sprite icepeaSprite(icepeaTexture);

  static sf::Texture &repeaterpeaTexture = getTexture("assets/Plants/Repeaterpea.png");
  static sf::Sprite repeaterpeaSprite(repeaterpeaTexture);

  static sf::Texture &tallNutTexture = getTexture("assets/Plants/tallnut/Tallnut_body.png");
  static sf::Sprite tallNutSprite(tallNutTexture);

  static sf::Texture &cherryBombTexture = getTexture("assets/Plants/cherrybomb.png");
  static sf::Sprite cherryBombSprite(cherryBombTexture);

  static sf::Texture &jalapenoTexture = getTexture("assets/Plants/jalapeno.png");
  static sf::Sprite jalapenoSprite(jalapenoTexture);

  static sf::Texture &potatoMineTexture = getTexture("assets/Plants/potatoMine2.png");
  static sf::Sprite potatoMineSprite(potatoMineTexture);

  static sf::Texture &iceShroomTexture = getTexture("assets/Plants/iceshroom.png");
  static sf::Sprite iceShroomSprite(iceShroomTexture);

  static sf::Texture &squashTexture = getTexture("assets/Plants/squash.png");
  static sf::Sprite squashSprite(squashTexture);

  static sf::Texture &puffShroomTexture = getTexture("assets/Plants/puff_shroom.png");
  static sf::Sprite puffShroomSprite(puffShroomTexture);

  static sf::Texture &regularZombieTexture = getTexture("assets/Zombies/Regular/zombie.png");
  static sf::Sprite regularZombieSprite(regularZombieTexture);

  static sf::Texture &coneheadZombieTexture = getTexture("assets/Zombies/Conehead/zombie.png");
  static sf::Sprite coneheadZombieSprite(coneheadZombieTexture);

  static sf::Texture &bucketheadZombieTexture = getTexture("assets/Zombies/Buckethead/zombie.png");
  static sf::Sprite bucketheadZombieSprite(bucketheadZombieTexture);

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

    //potatoMineSprite.setScale({0.135f, 0.135f});
    potatoMineSprite.setScale({0.21f, 0.21f});

    iceShroomSprite.setScale({0.8f, 0.8f});

    squashSprite.setScale({0.21f, 0.21f});

    puffShroomSprite.setScale({0.13f, 0.13f});


    // Zombies

    regularZombieSprite.setTextureRect({{56, 16}, {90, 144}});
    coneheadZombieSprite.setTextureRect({{56, 16}, {90, 144}});
    bucketheadZombieSprite.setTextureRect({{56, 16}, {90, 144}});

    regularZombieSprite.setScale({0.5, 0.5});
    coneheadZombieSprite.setScale({0.5, 0.5});
    bucketheadZombieSprite.setScale({0.5, 0.5});

    float shift = 630;

    zombiePackets.push({100, 5, "regular", {90.0f + (59.0f * 0) + shift, 11}, regularZombieSprite, static_cast<PlantType>(0)});
    zombiePackets.push({150, 5, "conehead", {90.0f + (59.0f * 1) + shift, 11}, coneheadZombieSprite, static_cast<PlantType>(1)});
    zombiePackets.push({200, 5, "buckethead", {90.0f + (59.0f * 2) + shift, 11}, bucketheadZombieSprite, static_cast<PlantType>(2)});

    runOnce = false;
  }

  packets.erase([](SeedPacket &s) {return true; }); // clear first
  for (int i = 0; i < types.size; i++) {
    float posX = 90.0f + (59.0f * i);
    switch (types[i]) {
    case PEASHOOTER:
      packets.push({100, 7.5f, "peashooter", {posX, 11}, peashooterSprite, PEASHOOTER});
      break;

    case SUN_FLOWER:
      packets.push({50, 7.5f, "sunflower", {posX, 11}, sunFlowerSprite, SUN_FLOWER});
      break;

    case WALLNUT:
      packets.push( {50, 30.0f, "wallnut", {posX, 11}, wallNutSprite, WALLNUT});
      break;

    case SNOWPEASHOOTER:
      packets.push({175, 7.5f, "peaice", {posX, 11}, icepeaSprite, SNOWPEASHOOTER});
      break;

    case REPEATERPEA:
      packets.push({200, 7.5f, "repeated", {posX, 11}, repeaterpeaSprite, REPEATERPEA});
      break;

    case TALLNUT:
      packets.push( {125, 30.0f, "tallnut", {posX, 11}, tallNutSprite, TALLNUT});
      break;

    case CHERRYBOMB:
      packets.push({150, 50.0f, "cherrybomb", {posX, 11}, cherryBombSprite, CHERRYBOMB});
      break;

    case JALAPENO:
      packets.push( {125, 50.0f, "jalapeno", {posX, 11}, jalapenoSprite, JALAPENO});
      break;

    case POTATOMINE:
      packets.push({ 25, 30.0f, "potatomine", {posX, 11}, potatoMineSprite, POTATOMINE });
      break;
    case ICESHROOM:
      packets.push({ 75, 50.0f, "iceshroom", {posX, 11}, iceShroomSprite, ICESHROOM });
      break;
    case SQUASH:
      packets.push({ 50, 30.0f, "squash", {posX, 11}, squashSprite, SQUASH });
      break;
    case PUFFSHROOM:
      packets.push({ 0, 7.5f, "puffshroom", {posX, 11}, puffShroomSprite, PUFFSHROOM });
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
      reloadDuration(reloadDurationValue),
  packetFlash(getTexture("assets/packets/PacketFlash.png")) {

  float scaleFactor = 1.0;
  enabledSprite.setPosition(position);
  enabledSprite.setScale({scaleFactor, scaleFactor});

  disabledSprite.setPosition(position);
  disabledSprite.setScale({ scaleFactor, scaleFactor });

  packetFlash.setPosition(position);
  packetFlash.setScale({ scaleFactor, scaleFactor });

  plantSprite.setOrigin(plantSprite.getLocalBounds().getCenter());

  plantShadow.setOrigin(plantShadow.getLocalBounds().size / 2.0f);

  sf::Vector2f scale = {50.f / 864 , 70.f / 1210};
  if(packetName == "regular") {
    enabledSprite.scale(scale);
    disabledSprite.scale(scale);
  }
  if(packetName == "conehead") {
    enabledSprite.scale(scale);
    disabledSprite.scale(scale);
  }
  if(packetName == "buckethead") {
    enabledSprite.scale(scale);
    disabledSprite.scale(scale);
  }

  

  if (plantType == ICESHROOM)
    plantShadow.setColor(sf::Color{255, 255, 255, 150});
  else
    plantShadow.setColor(sf::Color{255, 255, 255, 100});
}

void SeedPacket::update(float dt) {
  if(flashTimer > 0) flashTimer = std::max(0.0f, flashTimer - dt);
  if (reloadTimer > 0) {
    reloadTimer = std::max(0.0f, reloadTimer - dt);
    if (reloadTimer == 0.0f) {
      flashTimer = 0.8f;
    }
  }

  packetFlash.setColor(sf::Color(255, 255, 255, (
    (uint8_t)(0 + 255 * (flashTimer / 0.8f))
     )));


  if (Sun::sunBalance >= cost && reloadTimer == 0 && isMousePressed &&
      enabledSprite.getGlobalBounds().contains(mousePosition) && /*dayLevel.introTimer >= 21.0f*/true)
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
  window->draw(packetFlash);
}

void SeedPacket::drawSelectedPlant() {
  if (selected)
    window->draw(plantSprite);
}

void updateSeedPackets(float dt) {
  if(peer.type == Peer::Plants) {
    for (int i = 0; i < packets.size; i++) {
      packets[i].update(dt);
    }
  } else {
    for (int i = 0; i < zombiePackets.size; i++) {
      zombiePackets[i].update(dt);
    }
  }
}

void drawSeedPackets() {
  if(peer.type == Peer::Plants) {
    for (int i = 0; i < packets.size; i++) {
      packets[i].draw();
    }
  } else {
    for (int i = 0; i < zombiePackets.size; i++) {
      zombiePackets[i].draw();
    }
  }
}
