#include <Plants/SunFlower.hpp>
#include <Plants/Wallnut.hpp>
#include <Plants/Plant.hpp>
#include <globals.hpp>
#include <Plants/Peashooter.hpp>
#include <Plants/SnowpeaShooter.hpp>
#include <Plants/Repeaterpea.hpp>
#include <iostream>

float getPlantHealth(PlantType type){
  float health;
  switch (type){
    case SUN_FLOWER:
      health = SUN_FLOWER_HEALTH;
      break;
    case WALLNUT:
      health = WALLNUT_HEALTH;
      break;
    case PEASHOOTER:
      health = PEASHOOTER_HEALTH;
      break;
    case SNOWPEASHOOTER:
      health = SNOWPEASHOOTER_HEALTH;
      break;
    case REPEATERPEA:
      health = REPEATERPEA_HEALTH;
      break;
  }

  return health;
}

float getPlantTimer(PlantType type){
  float timer;
  switch (type){
    case SUN_FLOWER:
      timer = GENERATE_SUN_FLOWER_INTERVAL;
      break;
    case WALLNUT:
      timer = 0;
      break;
    case PEASHOOTER:
      timer = GENERATE_BULLET_TIMER;
      break;
    case SNOWPEASHOOTER:
      timer = GENERATE_SNOWBULLET_TIMER;
      break;
    case REPEATERPEA:
      timer = GENERATE_REPEATERBULLET_TIMER;
      break;
  }

  return timer;
}

Plant::Plant(PlantType type, sf::Vector2f position, int Row, sf::Texture &plantTexture, int width, int height, int numberOfFrames, sf::Vector2f scale) : sprite(plantTexture){
  sprite.setTextureRect({{0, 0}, {width, height}});
  sprite.setScale(scale);

  health = getPlantHealth(type);
  plantType = type;
  timer = getPlantTimer(type);
  row = Row;
  state = 0;
  
  float speed = 0.07f * 16.0f / numberOfFrames;
  sheet = Spritesheet{&sprite, width, height, numberOfFrames, speed};

  sprite.setOrigin(sprite.getLocalBounds().size / 2.0f);
  sprite.setPosition({position.x, position.y});  
}

void Plant::update(float dt) {
  switch (plantType) {
    case SUN_FLOWER:
      updateSunFlower(*this, dt);
      break;
    case WALLNUT:
      updateWallnut(*this, dt);
      break;
    case PEASHOOTER:
      updatePeashooter(*this, dt);
      break;
    case SNOWPEASHOOTER:
      updateSnowpeaShooter(*this, dt);
      break;
    case REPEATERPEA:
      updateRepeaterpea(*this, dt);
      break;
  }
}

void Plant::draw() {
  switch (plantType) {
    case SUN_FLOWER:
      drawSunFlower(*this);
      break;
    case WALLNUT:
      drawWallnut(*this);
      break;
    case PEASHOOTER:
      drawPeashooter(*this);
      break;
    case SNOWPEASHOOTER:
      drawSnowpeaShooter(*this);
      break;
    case REPEATERPEA:
      drawRepeaterpea(*this);
      break;
  }
}
