#pragma once

#include <SFML/Graphics.hpp>
#include <functional>


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

extern const sf::Vector2f SUN_COLLECTION_SITE;
extern bool isPaused;
extern int gameState;
extern int homeState;
void onClick(sf::Text &, std::function<void()>);
//bool onClickSun(
//    Sun *&sun,
    //std::function<void(Sun *s)> action); // Handle Sun click detection
float updateSlider(
    Slider &slider); // Updates then Returns slider value ranged (0->100)
void updateCheckbox(Checkbox &cb, bool &target);
float randomRange(float x, float y); // Return random value from x to y
