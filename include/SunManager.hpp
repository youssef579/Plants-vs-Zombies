#pragma once

// #include <SunManager.hpp>
#include <Animation.hpp>
#include <AssetsManager.hpp>
#include <Audio.hpp>
#include <Window.hpp>
#include <globals.hpp>

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
  Spritesheet sheet;
};

extern int SunBalance;
extern const sf::Vector2f SUN_COLLECTION_SITE;

void manageSuns();
void drawSun();
void updateSun();
void generateSun(float x, float y, int value);
void spawnSun(int value = 25);
void collectSun(Sun *sun);
bool onClickSun(Sun *&sun, std::function<void(Sun *s)>);
