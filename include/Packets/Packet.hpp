#pragma once

#include <Array.hpp>
#include <SFML/Graphics.hpp>

struct SeedPacket {
  int cost;
  float reloadDuration;
  float reloadTimer;
  sf::Sprite enabledSprite;
  sf::Sprite disabledSprite;
  sf::Sprite plantSprite;
  bool selected;

  SeedPacket(int costValue, float reloadDurationValue, std::string plantName, sf::Vector2f position, sf::Sprite preview);

  void update(float dt);

  void draw();
};

void initPackets();

extern Array<SeedPacket> packets;
