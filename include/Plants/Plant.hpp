#pragma once
#include <globals.hpp>
#include <Window.hpp>
#include <AssetsManager.hpp>
#include <Audio.hpp>
#include <Animation.hpp>

enum PlantType {
  SUN_FLOWER,
  WALLNUT,
  PEASHOOTER,
  SNOWPEASHOOTER,
  REPEATERPEA
}; // will add the other sorts of plants

struct Plant {
  sf::Sprite sprite;
  float health;
  PlantType plantType;
  float timer; //  the time remaining to generate a sun or a bullet based on "plantType" or state of wallnut
  int row;
  int state;   // could be used in repeater or other plants
  Spritesheet sheet;

  void update(float dt);

  void draw();
};
