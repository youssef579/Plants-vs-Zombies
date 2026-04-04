#pragma once

#include <SFML/Graphics.hpp>

struct Checkbox {
  sf::Sprite box;
  sf::Sprite mark;
  float x;
  float y;
  bool checked;
};

void updateCheckbox(Checkbox &checkbox, bool &target);
