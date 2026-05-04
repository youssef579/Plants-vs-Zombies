#pragma once

#include <Array.hpp>
#include <SFML/Graphics.hpp>
#include <Plants/Plant.hpp>


struct SeedPacket {
  int cost;
  float reloadDuration;
  float reloadTimer;
  sf::Sprite enabledSprite;
  sf::Sprite disabledSprite;
  sf::Sprite plantSprite;
  sf::Sprite plantShadow;
  sf::Sprite packetFlash;
  PlantType plantType;
  bool selected;

  float flashTimer = 0;
  SeedPacket(int costValue, float reloadDurationValue, std::string plantName, sf::Vector2f position, sf::Sprite preview, PlantType planttype);

  void update(float dt);

  void draw();

  void drawSelectedPlant();
};

void fillPackets(Array<PlantType> &types);
void updateSeedPackets(float dt);
void drawSeedPackets();


extern Array<SeedPacket> packets;
extern Array<PlantType> chosenPlants;
