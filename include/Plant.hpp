#pragma once
#include <globals.hpp>
#include <Window.hpp>
#include <AssetsManager.hpp>
#include <Audio.hpp>
#include <Animation.hpp>

struct Plant{
  sf::Sprite sprite;
  float health;
  enum PlantType {SUN_FLOWER}; // will add the other sorts of plants
  PlantType plantType;
  Spritesheet sheet;
  float Timer; //  the time remaining to generate a sun or a bullet based on "plantType"
};
