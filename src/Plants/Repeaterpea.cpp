#include <Plants/Repeaterpea.hpp>
#include <Plants/Plant.hpp>
#include <Bullet.hpp>
#include <Zombies/Zombie.hpp>

void updateRepeaterpea(Plant& repeaterpea, float dt) {
  //animateSpritesheet(repeaterpea.sheet, dt);
  repeaterpea.reAnimator.update(dt);

  // Zombie detection
  if (Zombie::isZombieAliveInRow(repeaterpea.row, repeaterpea.reAnimator.getPosition().x + 70.0f)) // only shoot when there is a zombie in the row
    repeaterpea.timer -= dt;
  else {
    repeaterpea.reAnimator.stopAnimation("anim_shooting");
    repeaterpea.timer = GENERATE_REPEATERBULLET_TIMER;
    repeaterpea.state = 0;
  }

  repeaterpea.blinkTimer -= dt;

  if (repeaterpea.blinkTimer <= 0) {
    repeaterpea.reAnimator.playAnimation("anim_blink", LoopType::PlayOnce);
    repeaterpea.blinkTimer = randomRange(3.0f, 4.5f);
  }

  if (repeaterpea.timer <= 0.8f && repeaterpea.state == 0) {
    repeaterpea.reAnimator.playAnimation("anim_shooting", LoopType::PlayOnce);
    repeaterpea.state = 1;
  }

  if (repeaterpea.timer <= 0 && repeaterpea.state == 1) {
    //sf::Vector2f currPOS = repeaterpea.sprite.getPosition();
    //repeaterpea.reAnimator.playAnimation("anim_shooting", false);
    //currTransforms["anim_face"] = currTransforms[19]
    sf::Vector2f currPOS =
    {
      ((sf::Transform().translate({repeaterpea.reAnimator.x, repeaterpea.reAnimator.y}))
        * ReAnimator::transformToSFML(repeaterpea.reAnimator.curTransforms[19])).getMatrix()[12] + 50,

        ((sf::Transform().translate({repeaterpea.reAnimator.x, repeaterpea.reAnimator.y}))
        * ReAnimator::transformToSFML(repeaterpea.reAnimator.curTransforms[19])).getMatrix()[13] + 7

    };
    sounds.play((rand() & 1) ? "Throw1" : "Throw2");
    Bullet newBullet = { PEA,{currPOS.x,currPOS.y},repeaterpea.row };
    bullets.push(newBullet);

    repeaterpea.state = 2;
    repeaterpea.timer = GENERATE_SECOND_REPEATERBULLET_TIMER;
  }
  if (repeaterpea.timer <= 0 && repeaterpea.state == 2){
    //repeaterpea.reAnimator.stopAnimation("anim_shooting");
    //repeaterpea.reAnimator.playAnimation("anim_shooting", false);
    //sf::Vector2f currPOS = repeaterpea.sprite.getPosition();
    //currTransforms["anim_face"] = currTransforms[19]

    sf::Vector2f currPOS =
    {
      ((sf::Transform().translate({repeaterpea.reAnimator.x, repeaterpea.reAnimator.y}))
        * ReAnimator::transformToSFML(repeaterpea.reAnimator.curTransforms[19])).getMatrix()[12] + 50,

        ((sf::Transform().translate({repeaterpea.reAnimator.x, repeaterpea.reAnimator.y}))
        * ReAnimator::transformToSFML(repeaterpea.reAnimator.curTransforms[19])).getMatrix()[13] + 7

    };
    sounds.play((rand() & 1) ? "Throw1" : "Throw2");
    Bullet newBullet = { PEA,{currPOS.x,currPOS.y},repeaterpea.row };
    bullets.push(newBullet);

    repeaterpea.state = 0;
    repeaterpea.timer = GENERATE_REPEATERBULLET_TIMER;
  }
}

void drawRepeaterpea(Plant& repeaterpea) {
  //window->draw(repeaterpea.sprite);
}
