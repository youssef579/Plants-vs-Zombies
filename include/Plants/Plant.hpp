#pragma once
#include <globals.hpp>
#include <Window.hpp>
#include <AssetsManager.hpp>
#include <Audio.hpp>
#include <Animation.hpp>
#include <ReAnimation/ReAnimation.hpp>

const float PLANT_BLINK_INTERVAL = 2.0f;

enum PlantType {
  SUN_FLOWER,
  PEASHOOTER,
  WALLNUT,
  SNOWPEASHOOTER,
  REPEATERPEA,
  TALLNUT
}; // will add the other sorts of plants

float getPlantHealth(PlantType type);

float getPlantTimer(PlantType type);

struct Plant {
  ReAnimator reAnimator;
  float health;
  PlantType plantType;
  float timer; //  the time remaining to generate a sun or a bullet based on "plantType" or state of wallnut
  float blinkTimer;
  int row;
  int state;   // could be used in repeater or other plants

  Plant(PlantType type, sf::Vector2f position, int Row, ReAnimationDefinition *def);

  void update(float dt);

  void draw();
};
