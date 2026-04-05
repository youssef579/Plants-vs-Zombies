#pragma once
#include <sfml/Graphics.hpp>
#include <Window.hpp>
#include <random>


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

float updateSlider(Slider& slider);

void updateCheckbox(Checkbox& cb, bool& target);