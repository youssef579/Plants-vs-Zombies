#pragma once
#include <globals.hpp>

struct Spritesheet {
  sf::Sprite* sprite;
  int frameWidth;
  int frameHeight;
  int frameCount;
  float frameDuration;
  float timer = 0.0f;
};


void animateSpritesheet(Spritesheet& sheet, float dt);