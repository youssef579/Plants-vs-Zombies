#include <Plants/SnowpeaShooter.hpp>
#include <Plants/Plant.hpp>
#include <Bullet.hpp>
#include <Zombies/Zombie.hpp>

void updateSnowpeaShooter(Plant& snowpeaShooter, float dt) {
  snowpeaShooter.reAnimator.update(dt);

  // Zombie detection
  if (Zombie::isZombieAliveInRow(snowpeaShooter.row, snowpeaShooter.reAnimator.getPosition().x + 70.0f)) // only shoot when there is a zombie in the row
    snowpeaShooter.timer -= dt;
  else {
    snowpeaShooter.reAnimator.stopAnimation("anim_shooting");
    snowpeaShooter.timer = GENERATE_SNOWBULLET_TIMER;
  }

  snowpeaShooter.blinkTimer -= dt;

  // blink
  if (snowpeaShooter.blinkTimer <= 0) {
    snowpeaShooter.reAnimator.playAnimation("anim_blink", LoopType::PlayOnce);
    snowpeaShooter.blinkTimer = randomRange(3.0f, 4.5f);
  }

  //generate bullet
  if (snowpeaShooter.timer <= 0) {  
    if (snowpeaShooter.timer > -1000) {
      snowpeaShooter.reAnimator.playAnimation("anim_shooting", LoopType::PlayOnce);
      snowpeaShooter.timer = -10000;

    }
    
    if (snowpeaShooter.timer <= -10000 - 0.4) { // -10000 - time after animation start to actual shoot
      snowpeaShooter.timer = GENERATE_SNOWBULLET_TIMER;
      sf::Vector2f currPOS =
      {
        ((sf::Transform().translate({snowpeaShooter.reAnimator.x, snowpeaShooter.reAnimator.y}))
          * ReAnimator::transformToSFML(snowpeaShooter.reAnimator.curTransforms[16])).getMatrix()[12] + 50,

          ((sf::Transform().translate({snowpeaShooter.reAnimator.x, snowpeaShooter.reAnimator.y}))
          * ReAnimator::transformToSFML(snowpeaShooter.reAnimator.curTransforms[16])).getMatrix()[13] + 7

      };
      sounds.play("ThrowSnowPea");
      Bullet newBullet = { SNOWPEA,{currPOS.x,currPOS.y},snowpeaShooter.row };
      bullets.push(newBullet);

    }

  }

}

void drawSnowpeaShooter(Plant& snowpeaShooter) {
  //snowpeaShooter.reAnimator.drawHitbox();
  //window->draw(snowpeaShooter.sprite);
}
