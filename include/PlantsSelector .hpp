#pragma once

#include <SFML/Graphics.hpp>

struct PlantsSelector {

  sf::Sprite* backSprite = nullptr;
  sf::Sprite* playBtn = nullptr;

  sf::Texture playBtnNormal;
  sf::Texture playBtnHover;

  float currX;
  float tarX;
  float speed;
  bool isVisible;
  bool isSlidingOut;

};

extern PlantsSelector plantSelector;

void initSelector();
void slideIn();
void slideOut();
void updateSelector(float dt, sf::RenderWindow& window);
void drawSelector(sf::RenderWindow& window);
