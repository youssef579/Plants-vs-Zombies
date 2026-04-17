#include <Plants/SnowpeaShooter.hpp>
#include <Plants/Plant.hpp>
#include<Bullet.hpp>

/*Plant createSnowpeaShooter(float x, float y, int row) {
  static sf::Texture& snowpeaShooterTexture = getTexture("assets/Plants/Icepea.png");
  // icepeaTexture.setSmooth(true);

  sf::Sprite snowpeaShooterSprite(snowpeaShooterTexture);
  snowpeaShooterSprite.setTextureRect({ {0,0},{353, 368}});
  snowpeaShooterSprite.setScale({0.218, 0.217});

  Plant plant = {snowpeaShooterSprite, SNOWPEASHOOTER_HEALTH, PlantType::SNOWPEASHOOTER, GENERATE_SNOWBULLET_TIMER, row,0, nullptr};
  Spritesheet snowpeaShooterSheet = Spritesheet{&plant.sprite, 353, 368, 25, 0.05f};
  plant.sheet = snowpeaShooterSheet;

  plant.sprite.setOrigin(plant.sprite.getLocalBounds().size / 2.0f);
  plant.sprite.setPosition({x, y});

  return plant;
}*/

void updateSnowpeaShooter(Plant& snowpeaShooter, float dt) {
  //animateSpritesheet(snowpeaShooter.sheet, dt);
  snowpeaShooter.reAnimator.update(dt);

  snowpeaShooter.timer -= dt;
  snowpeaShooter.blinkTimer -= dt;

  if (snowpeaShooter.blinkTimer <= 0) {
    snowpeaShooter.reAnimator.playAnimation("anim_blink", false);
    snowpeaShooter.blinkTimer = randomRange(2.0f, 3.0f);
  }

  if (snowpeaShooter.timer <= 0) {
    //peashooter.timer = GENERATE_BULLET_TIMER;
    if (snowpeaShooter.timer > -1000) {
      snowpeaShooter.reAnimator.playAnimation("anim_shooting", false);
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

      Bullet newBullet = { SNOWPEA,{currPOS.x,currPOS.y},snowpeaShooter.row };
      bullets.push(newBullet);

    }

  }

}

void drawSnowpeaShooter(Plant& snowpeaShooter) {
  //snowpeaShooter.reAnimator.drawHitbox();
  //window->draw(snowpeaShooter.sprite);
}
