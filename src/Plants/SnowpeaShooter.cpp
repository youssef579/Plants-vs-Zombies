#include <Plants/SnowpeaShooter.hpp>
#include <Plants/Plant.hpp>
#include <Bullet.hpp>
#include <Zombies/Zombie.hpp>

void updateSnowpeaShooter(Plant& snowpeaShooter, float dt) {
  //animateSpritesheet(snowpeaShooter.sheet, dt);
  snowpeaShooter.reAnimator.update(dt);

  // Zombie detection
  if (Zombie::isZombieAliveInRow(snowpeaShooter.row, snowpeaShooter.reAnimator.getPosition().x + 70.0f)) // only shoot when there is a zombie in the row
    snowpeaShooter.timer -= dt;
  else {
    snowpeaShooter.reAnimator.stopAnimation("anim_shooting");
    snowpeaShooter.timer = GENERATE_SNOWBULLET_TIMER;
  }

  snowpeaShooter.blinkTimer -= dt;

  if (snowpeaShooter.blinkTimer <= 0) {
    snowpeaShooter.reAnimator.playAnimation("anim_blink", LoopType::PlayOnce);
    snowpeaShooter.blinkTimer = randomRange(3.0f, 4.5f);
  }

  if (snowpeaShooter.timer <= 0) {
    //peashooter.timer = GENERATE_BULLET_TIMER;
    if (snowpeaShooter.timer > -1000) {
      snowpeaShooter.reAnimator.playAnimation("anim_shooting", LoopType::PlayOnce);
      snowpeaShooter.timer = -10000;

    }
    //peashooter.reAnimator.effectiveTransforms
    if (snowpeaShooter.timer <= -10000 - 0.8) { // -10000 - time after animation start to actual shoot
      snowpeaShooter.timer = GENERATE_SNOWBULLET_TIMER;

      sf::Vector2f currPOS =
      {
        ((sf::Transform().translate({snowpeaShooter.reAnimator.x, snowpeaShooter.reAnimator.y}))
          * ReAnimator::transformToSFML(snowpeaShooter.reAnimator.curTransforms["anim_face"])).getMatrix()[12] + 50,

          ((sf::Transform().translate({snowpeaShooter.reAnimator.x, snowpeaShooter.reAnimator.y}))
          * ReAnimator::transformToSFML(snowpeaShooter.reAnimator.curTransforms["anim_face"])).getMatrix()[13] + 7

      };
      sounds.play((rand() & 1) ? "Throw1" : "Throw2");
      Bullet newBullet = { SNOWPEA,{currPOS.x,currPOS.y},snowpeaShooter.row };
      bullets.push(newBullet);

    }

  }

}

void drawSnowpeaShooter(Plant& snowpeaShooter) {
  //snowpeaShooter.reAnimator.drawHitbox();
  //window->draw(snowpeaShooter.sprite);
}
