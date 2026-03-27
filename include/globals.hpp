#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

struct Sun {
  sf::Sprite sprite;
  int value;
  int state; // 0 -> falling(collectable), 1 -> on ground (collectable), 2 ->
             // collected(non-collecatble)
  float groundTimer; // time to stay on ground before auto-collection
  float distFromCollectionSite;
  sf::Vector2f direction;
  float fadeFactor; // Rate of fading according to initial distance (to prevent
                    // sudden fades)
  int currentFrame = 0;    // Index of the current frame
  float frameTimer = 0.03; // Frame duration
};

struct Slider { // Settings Slider
  sf::Sprite sprite;
  float y;          // y position
  float lowerBound; // x position of slider start
  float upperBound; // x position of slider end
  float length;     // upper-lower (can be automated later)
  bool isHolding;   // whether the slider is currently held
};

extern const sf::Vector2f SUN_COLLECTION_SITE;
extern bool isPaused;
extern int gameState;
void onClick(sf::Text &, std::function<void()>);
bool onClickSun(
    Sun *&sun,
    std::function<void(Sun *s)> action); // Handle Sun click detection
float updateSlider(
    Slider &slider); // Updates then Returns slider value ranged (0->100)
float randomRange(float x, float y); // Return random value from x to y
