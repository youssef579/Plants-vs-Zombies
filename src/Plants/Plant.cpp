#include <Plants/SunFlower.hpp>
#include <Plants/Wallnut.hpp>
#include <Plants/Plant.hpp>
#include <globals.hpp>

void Plant::update(float dt) {
  switch (plantType) {
    case SUN_FLOWER:
      updateSunFlower(*this, dt);
      break;
    case WALLNUT:
      updateWallnut(*this, dt);
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
  }
}
