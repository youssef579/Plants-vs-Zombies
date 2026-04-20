#pragma once

#include <SFML/Graphics.hpp>

struct Shovel {
  sf::Sprite *shovelBank   = nullptr;
  sf::Sprite *shovel       = nullptr;
  sf::Sprite *movingShovel = nullptr;
  bool selected;

  //Shovel();

  void init();

  void update();

  void drawBank();

  void drawMovingShovel();
};

extern Shovel shovel;
