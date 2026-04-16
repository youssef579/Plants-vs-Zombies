#include <Plants/SunFlower.hpp>
#include <Plants/Wallnut.hpp>
#include <Plants/Plant.hpp>
#include <globals.hpp>
#include <Plants/Peashooter.hpp>
#include <Plants/SnowpeaShooter.hpp>
#include <Plants/Repeaterpea.hpp>

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
