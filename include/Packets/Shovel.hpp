#pragma once

#include <SFML/Graphics.hpp>

struct Shovel {
  sf::Sprite shovelBank;
  sf::Sprite shovel;
  sf::Sprite movingShovel;
  bool selected;

  Shovel();

  void update();

  void draw();
};

