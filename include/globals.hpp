#pragma once

#include <SFML/Graphics.hpp>
#include <functional>



struct Sun {
  sf::Sprite sprite;
  int value;
  int state; //0 -> falling(collectable), 1 -> on ground (collectable), 2 -> collected(non-collecatble)
  float groundTimer; //time to stay on ground before auto-collection
  float distFromCollectionSite;
  sf::Vector2f direction;
};
extern const sf::Vector2f SUN_COLLECTION_SITE;

void onClick(sf::Text &, std::function<void()>);
bool onClickSun(Sun*& sun, std::function<void(Sun* s)> action); //Handle Sun click detection