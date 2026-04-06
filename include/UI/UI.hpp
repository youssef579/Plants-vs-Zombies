#pragma once
#include <SFML/Graphics.hpp>
#include <Window.hpp>



struct Slider {      // Settings Slider
  sf::Sprite sprite;
  float y;           // y position
  float lowerBound;  // x position of slider start
  float upperBound;  // x position of slider end
  float length;      // upper - lower (can be automated later)
  bool isHolding;    // whether the slider is currently held
};

struct Checkbox {
  sf::Sprite box;
  sf::Sprite mark;
  float x;
  float y;
  bool checked;
};

float updateSlider(Slider &slider); // Check for click & return value (0->100)

void updateCheckbox(Checkbox &cb, bool &target);