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

float getPlantHealth(PlantType type);

float getPlantTimer(PlantType type);

struct Plant {
  sf::Sprite sprite;
  float health;
  PlantType plantType;
  float timer; //  the time remaining to generate a sun or a bullet based on "plantType" or state of wallnut
  int row;
  int state;   // could be used in repeater or other plants
  Spritesheet sheet;

  Plant(PlantType type, sf::Vector2f position, int Row, sf::Texture &plantTexture, int width, int height, int numberOfFrames, sf::Vector2f scale);

  void update(float dt);

  void draw();
};
