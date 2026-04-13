#pragma once
#include <globals.hpp>
#include <Window.hpp>
#include <AssetsManager.hpp>
#include <Audio.hpp>
#include <Animation.hpp>

enum PlantType {
  SUN_FLOWER,
  WALLNUT
}; // will add the other sorts of plants

struct Plant{
  sf::Sprite sprite;
  float health;
  PlantType plantType;
  float timer; //  the time remaining to generate a sun or a bullet based on "plantType" or state of wallnut
  int row;
  Spritesheet sheet;

  void update(float dt);

  void draw();
};
