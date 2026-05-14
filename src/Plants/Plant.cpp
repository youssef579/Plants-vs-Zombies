#include <Plants/SunFlower.hpp>
#include <Plants/Wallnut.hpp>
#include <Plants/Tallnut.hpp>
#include <Plants/Plant.hpp>
#include <Plants/CherryBomb.hpp>
#include <Plants/Jalapeno.hpp>
#include <Plants/Squash.hpp>
#include <globals.hpp>
#include <Plants/Peashooter.hpp>
#include <Plants/SnowpeaShooter.hpp>
#include <Plants/Repeaterpea.hpp>
#include <Plants/potatoMine.hpp>
#include <Plants/iceShroom.hpp>
#include <Plants/puffShroom.hpp>
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
    case TALLNUT:
      health = TALLNUT_HEALTH;
      break;
    case CHERRYBOMB:
      health = CHERRY_HEALTH;
      break;
    case JALAPENO:
      health = JALAPENO_HEALTH;
      break;
    case POTATOMINE:
      health = POTATO_MINE_HEALTH;
      break;
    case ICESHROOM:
      health = ICESHROOM_HEALTH;
      break;
    case SQUASH:
      health = SQUASH_HEALTH;
      break;
    case PUFFSHROOM:
      health = PUFFSHROOM_HEALTH;
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
    case TALLNUT:
      timer = 0;
      break;
    case CHERRYBOMB:
      timer = CHERRY_EXPLOSION_TIMER;
      break;
    case JALAPENO:
      timer = JALAPENO_EXPLOSION_TIMER;
      break;
    case POTATOMINE:
      timer = POTATO_MINE_TIMER_BEFORE_EXPLOSION;
      break;
    case ICESHROOM:
      timer = ICESHROOM_EXPLOSION_TIMER;
      break;
    case SQUASH:
      timer = 0;
      break;
    case PUFFSHROOM:
      timer = GENERATE_PUFFSHROOM_BULLET_TIMER;
      break;
  }

  return timer;
}


Plant::Plant(PlantType type, sf::Vector2f position, int Row, int Col, ReAnimationDefinition *def)
  : reAnimator(def, position.x, position.y, window){
  health = getPlantHealth(type);
  plantType = type;
  timer = getPlantTimer(type);
  blinkTimer = PLANT_BLINK_INTERVAL;
  row = Row;
  col = Col;
  state = 0;

  reAnimator.setPosition(position);
  switch (plantType) {
  case SUN_FLOWER:
    reAnimator.playAnimation("idle");
    timer--;
    break;
  case WALLNUT:
    reAnimator.playAnimation("anim_idle");
    break;
  case PEASHOOTER:
    reAnimator.playAnimation("anim_idle");
    reAnimator.playAnimation("anim_head_idle");
    reAnimator.animSpeedMulti = 2.0f;
    break;
  case SNOWPEASHOOTER:
    reAnimator.playAnimation("anim_idle");
    reAnimator.playAnimation("anim_head_idle");
    reAnimator.animSpeedMulti = 2.0f;
    break;
  case REPEATERPEA:
    reAnimator.playAnimation("anim_idle");
    reAnimator.playAnimation("anim_head_idle");
    reAnimator.animSpeedMulti = 2.0f;
    break;
  case TALLNUT:
    reAnimator.playAnimation("anim_idle");
    break;
  case CHERRYBOMB: 
    reAnimator.playAnimation("anim_explode", LoopType::PlayOnce);
    break;
  case JALAPENO:
    reAnimator.playAnimation("anim_explode", LoopType::PlayOnce);
    break;
  case POTATOMINE:
    reAnimator.playAnimation("anim_idle", LoopType::Loop);
    break;
  case ICESHROOM:
    reAnimator.playAnimation("anim_idle", LoopType::Loop);
    break;
  case SQUASH:
    reAnimator.playAnimation("anim_idle", LoopType::Loop);
    break;
  case PUFFSHROOM:
    reAnimator.playAnimation("anim_idle");
    break;
  }
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
    case TALLNUT:
      updateTallnut(*this, dt);
      break;
    case CHERRYBOMB:
      updateCherryBomb(*this, dt);
      break;
    case JALAPENO:
      updateJalapeno(*this, dt);
      break;
    case POTATOMINE:
      updatePotatoMine(*this, dt);
      break;
    case ICESHROOM:
      updateIceShroom(*this, dt);
      break;
    case SQUASH:
      updateSquash(*this, dt);
      break;
    case PUFFSHROOM:
      updatePuffShroom(*this, dt);
      break;
  }
}

void Plant::draw() {
  reAnimator.draw();
  //reAnimator.drawHitbox();
  switch (plantType) { // only sunflower is used, rest are for reassurance
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
    case TALLNUT:
      drawTallnut(*this);
      break;
    case CHERRYBOMB:
      drawCherryBomb(*this);
      break;
    case JALAPENO:
      drawJalapeno(*this);
      break;
    case POTATOMINE:
      drawPotatoMine(*this);
      break;
    case ICESHROOM:
      drawIceShroom(*this);
      break;
    case SQUASH:
      drawSquash(*this);
      break;
    case PUFFSHROOM:
      drawPuffShroom(*this);
      break;
  }
}
