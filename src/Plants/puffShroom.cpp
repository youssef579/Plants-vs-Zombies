#include <Plants/puffShroom.hpp>
#include <Plants/Plant.hpp>
#include <Bullet.hpp>
#include <Zombies/Zombie.hpp>


void updatePuffShroom(Plant& puffShroom, float dt) {
  puffShroom.reAnimator.update(dt);


  if (Zombie::isZombieAliveInRow(puffShroom.row, puffShroom.reAnimator.getPosition().x + 70.0f)) // only shoot when there is a zombie in the row
    puffShroom.timer -= dt;
  else {
    puffShroom.reAnimator.stopAnimation("anim_shooting");
    puffShroom.timer = GENERATE_PUFFSHROOM_BULLET_TIMER;
  }

  puffShroom.blinkTimer -= dt;
  if (puffShroom.blinkTimer <= 0) {
    puffShroom.reAnimator.playAnimation("anim_blink", LoopType::PlayOnce);
    puffShroom.blinkTimer = randomRange(3.0f, 4.5f);
  }

  if (puffShroom.timer <= 0) {
    if (puffShroom.timer > -10000) {
      puffShroom.reAnimator.playAnimation("anim_shooting", LoopType::PlayOnce);
      puffShroom.timer = -10000;

    }
    if (puffShroom.timer <= -10000-0.4) {
      puffShroom.timer = GENERATE_PUFFSHROOM_BULLET_TIMER;

      sf::Vector2f currPOS =
      {
        ((sf::Transform().translate({puffShroom.reAnimator.x, puffShroom.reAnimator.y}))
          * ReAnimator::transformToSFML(puffShroom.reAnimator.curTransforms[14])).getMatrix()[12] + 50,

          ((sf::Transform().translate({puffShroom.reAnimator.x, puffShroom.reAnimator.y}))
          * ReAnimator::transformToSFML(puffShroom.reAnimator.curTransforms[14])).getMatrix()[13] + 7

      };

      sounds.play((rand()&1) ? "Throw1" : "Throw2");
      Bullet newBullet = { SHROOM,{currPOS.x,currPOS.y},puffShroom.row };
      bullets.push(newBullet);

    }
  }
}

void drawPuffShroom(Plant& peashooter) {
  
}
