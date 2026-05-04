#include <Plants/Peashooter.hpp>
#include <Plants/Plant.hpp>
#include <Bullet.hpp>
#include <Zombies/Zombie.hpp>


void updatePeashooter(Plant& peashooter, float dt) {
  //animateSpritesheet(peashooter.sheet, dt);
  peashooter.reAnimator.update(dt);


  // Zombie detection
  if (Zombie::isZombieAliveInRow(peashooter.row, peashooter.reAnimator.getPosition().x + 70.0f)) // only shoot when there is a zombie in the row
    peashooter.timer -= dt;
  else {
    peashooter.reAnimator.stopAnimation("anim_shooting");
    peashooter.timer = GENERATE_BULLET_TIMER;
  }



  peashooter.blinkTimer -= dt;
  if (peashooter.blinkTimer <= 0) {
    peashooter.reAnimator.playAnimation("anim_blink", LoopType::PlayOnce);
    peashooter.blinkTimer = randomRange(3.0f, 4.5f);
  }

  if (peashooter.timer <= 0) {
    //peashooter.timer = GENERATE_BULLET_TIMER;
    if (peashooter.timer > -10000) {
      peashooter.reAnimator.playAnimation("anim_shooting", LoopType::PlayOnce);
      peashooter.timer = -10000;

    }
    //peashooter.reAnimator.effectiveTransforms
    if (peashooter.timer <= -10000-0.4) { // -10000 - time after animation start to actual shoot
      peashooter.timer = GENERATE_BULLET_TIMER;
      /*sf::Vector2f currPOS =
      {
        ((sf::Transform().translate(peashooter.reAnimator.getPosition()))
          * peashooter.reAnimator.effectiveTransforms["anim_mouth"]).getMatrix()[12],

          ((sf::Transform().translate(peashooter.reAnimator.getPosition()))
          *peashooter.reAnimator.effectiveTransforms["anim_mouth"]).getMatrix()[13]

      };*/

      //currTransforms["anim_face"] = currTransforms[14]
      sf::Vector2f currPOS =
      {
        ((sf::Transform().translate({peashooter.reAnimator.x, peashooter.reAnimator.y}))
          * ReAnimator::transformToSFML(peashooter.reAnimator.curTransforms[14])).getMatrix()[12] + 50,

          ((sf::Transform().translate({peashooter.reAnimator.x, peashooter.reAnimator.y}))
          * ReAnimator::transformToSFML(peashooter.reAnimator.curTransforms[14])).getMatrix()[13] + 7

      };

      sounds.play((rand()&1) ? "Throw1" : "Throw2");
      Bullet newBullet = { PEA,{currPOS.x,currPOS.y},peashooter.row };
      bullets.push(newBullet);

    }

    //sf::Vector2f currPOS = peashooter.sprite.getPosition();
    /*if (peashooter.timer <= -2.0f) {
      peashooter.timer = GENERATE_BULLET_TIMER;
      sf::Vector2f currPOS = peashooter.reAnimator.getPosition();
      Bullet newBullet = { PEA,{currPOS.x + 25.0f,currPOS.y - 25.0f},peashooter.row };
      bullets.push(newBullet);
    }*/
  }
}

void drawPeashooter(Plant& peashooter) {
  //peashooter.reAnimator.drawHitbox();
  //peashooter.reAnimator.drawHitbox();
  //static sf::RectangleShape rec({3, 3});
  //rec.setPosition({300, 300});
  //rec.setFillColor(sf::Color(0, 255, 0, 255));
}
