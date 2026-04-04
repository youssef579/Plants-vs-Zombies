#pragma once

#include <SFML/Graphics.hpp>

struct Slider { // Settings Slider
  sf::Sprite sprite;
  float y;          // y position
  float lowerBound; // x position of slider start
  float upperBound; // x position of slider end
  float length;     // upper - lower (can be automated later)
  bool isHolding;   // whether the slider is currently held
};

float updateSlider(
    Slider &slider); // Updates then Returns slider value ranged (0->100)
