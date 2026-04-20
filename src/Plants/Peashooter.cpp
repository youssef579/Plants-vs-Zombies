#include <Plants/Peashooter.hpp>
#include <Plants/Plant.hpp>
#include <Bullet.hpp>

void updatePeashooter(Plant& peashooter, float dt) {
  //animateSpritesheet(peashooter.sheet, dt);
  peashooter.reAnimator.update(dt);

  peashooter.timer -= dt;
  peashooter.blinkTimer -= dt;

  if (peashooter.blinkTimer <= 0) {
    peashooter.reAnimator.playAnimation("anim_blink", LoopType::PlayOnce);
    peashooter.blinkTimer = randomRange(2.0f, 3.5f);
  }

  if (peashooter.timer <= 0) {
    //peashooter.timer = GENERATE_BULLET_TIMER;
    if (peashooter.timer > -1000) {
      peashooter.reAnimator.playAnimation("anim_shooting", LoopType::PlayOnce);
      peashooter.timer = -10000;

    }
    //peashooter.reAnimator.effectiveTransforms
    if (peashooter.timer <= -10000-0.8) { // -10000 - time after animation start to actual shoot
      peashooter.timer = GENERATE_BULLET_TIMER;
      /*sf::Vector2f currPOS =
      {
        ((sf::Transform().translate(peashooter.reAnimator.getPosition()))
          * peashooter.reAnimator.effectiveTransforms["anim_mouth"]).getMatrix()[12],

          ((sf::Transform().translate(peashooter.reAnimator.getPosition()))
          *peashooter.reAnimator.effectiveTransforms["anim_mouth"]).getMatrix()[13]

      };*/

      sf::Vector2f currPOS =
      {
        ((sf::Transform().translate({peashooter.reAnimator.x, peashooter.reAnimator.y}))
          * ReAnimator::transformToSFML(peashooter.reAnimator.curTransforms["anim_face"])).getMatrix()[12] + 50,

          ((sf::Transform().translate({peashooter.reAnimator.x, peashooter.reAnimator.y}))
          * ReAnimator::transformToSFML(peashooter.reAnimator.curTransforms["anim_face"])).getMatrix()[13] + 7

      };

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
  //static sf::RectangleShape rec({3, 3});
  //rec.setPosition({300, 300});
  //rec.setFillColor(sf::Color(0, 255, 0, 255));


  /*sf::Vector2f currPOS =
  {
    ((sf::Transform().translate({peashooter.reAnimator.x, peashooter.reAnimator.y}))
      * ReAnimator::transformToSFML(peashooter.reAnimator.curTransforms["anim_face"])).getMatrix()[12] + 50,

      ((sf::Transform().translate({peashooter.reAnimator.x, peashooter.reAnimator.y}))
      * ReAnimator::transformToSFML(peashooter.reAnimator.curTransforms["anim_face"])).getMatrix()[13] + 10

  };*/

  //std::cout << "currPos: (" << currPOS.x << ", " << currPOS.y << ")\n";
  //std::cout << "mouth_x: " << peashooter.reAnimator.curTransforms["idle_mouth"].x << "\n";
  //rec.setPosition(currPOS);
  //window->draw(rec);


  //window->draw(rec);

  //window->draw(peashooter.sprite);
}
