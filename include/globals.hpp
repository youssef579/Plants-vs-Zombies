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

  //Slider(sf::Sprite s, float Y, float lb, float ub, float l, bool hold)
    //:sprite(s), y(Y), lowerBound(lb), upperBound(ub), length(l), isHolding(hold)
  //{}
};

struct Checkbox {
  sf::Sprite box;
  sf::Sprite mark;
  float x;
  float y;
  bool checked;

  //Checkbox(sf::Sprite BOX, sf::Sprite MARK, float X, float Y, bool CHECKED):
  //box(BOX), mark(MARK), x(X), y(Y), checked(CHECKED)
  //{}

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
