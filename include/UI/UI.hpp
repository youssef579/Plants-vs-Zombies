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


struct PopupUI {
  sf::Sprite *sprite;

  float startY;
  float targetY;
  float speed;

  enum UIState { Idle, MovingDown, Waiting, MovingUp };
  UIState state = UIState::Idle;
  sf::Clock waitTimer;
  float waitDuration;

  void init(sf::Sprite *sp, sf::Vector2f startPos, float dropDistance, float moveSpeed, float wd);
  void trigger();
  void update(float dt);
};

float updateSlider(Slider &slider); // Check for click & return value (0->100)

bool updateCheckbox(Checkbox &cb, bool &target);

void drawTimeModifier(float dt);
