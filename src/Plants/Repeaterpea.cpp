#include <Plants/Repeaterpea.hpp>
#include <Plants/Plant.hpp>
#include<Bullet.hpp>

/*Plant createRepeaterpea(float x, float y, int row) {
  static sf::Texture& repeaterpeaTexture = getTexture("assets/Plants/Repeaterpea.png");
  repeaterpeaTexture.setSmooth(true);

  sf::Sprite repeaterpeaSprite(repeaterpeaTexture);
  repeaterpeaSprite.setTextureRect({ {0,0},{73, 71}});

  Plant plant = {repeaterpeaSprite, REPEATERPEA_HEALTH, PlantType::REPEATERPEA, GENERATE_REPEATERBULLET_TIMER, row, 0, nullptr};
  Spritesheet repeaterpeaSheet = Spritesheet{ &plant.sprite,73,71, 15,0.0426f };
  plant.sheet = repeaterpeaSheet;

  plant.sprite.setOrigin(plant.sprite.getLocalBounds().size / 2.0f);
  plant.sprite.setPosition({ x,y });

  return plant;
}*/

void updateRepeaterpea(Plant& repeaterpea, float dt) {
  //animateSpritesheet(repeaterpea.sheet, dt);
  repeaterpea.reAnimator.update(dt);

  repeaterpea.timer -= dt;
  repeaterpea.blinkTimer -= dt;

  if (repeaterpea.blinkTimer <= 0) {
    repeaterpea.reAnimator.playAnimation("anim_blink", false);
    repeaterpea.blinkTimer = randomRange(2.0f, 3.5f);
  }

  if (repeaterpea.timer <= 0.8f && repeaterpea.state == 0) {
    repeaterpea.reAnimator.playAnimation("anim_shooting", false);
    repeaterpea.state = 1;
  }

  if (repeaterpea.timer <= 0 && repeaterpea.state == 1) {
    //sf::Vector2f currPOS = repeaterpea.sprite.getPosition();
    //repeaterpea.reAnimator.playAnimation("anim_shooting", false);
    sf::Vector2f currPOS =
    {
      ((sf::Transform().translate({repeaterpea.reAnimator.x, repeaterpea.reAnimator.y}))
        * ReAnimator::transformToSFML(repeaterpea.reAnimator.curTransforms["anim_face"])).getMatrix()[12] + 50,

        ((sf::Transform().translate({repeaterpea.reAnimator.x, repeaterpea.reAnimator.y}))
        * ReAnimator::transformToSFML(repeaterpea.reAnimator.curTransforms["anim_face"])).getMatrix()[13] + 7

    };
    Bullet newBullet = { PEA,{currPOS.x,currPOS.y},repeaterpea.row };
    bullets.push(newBullet);

    repeaterpea.state = 2;
    repeaterpea.timer = GENERATE_SECOND_REPEATERBULLET_TIMER;
  }
  if (repeaterpea.timer <= 0 && repeaterpea.state == 2){
    //repeaterpea.reAnimator.stopAnimation("anim_shooting");
    //repeaterpea.reAnimator.playAnimation("anim_shooting", false);
    //sf::Vector2f currPOS = repeaterpea.sprite.getPosition();
    sf::Vector2f currPOS =
    {
      ((sf::Transform().translate({repeaterpea.reAnimator.x, repeaterpea.reAnimator.y}))
        * ReAnimator::transformToSFML(repeaterpea.reAnimator.curTransforms["anim_face"])).getMatrix()[12] + 50,

        ((sf::Transform().translate({repeaterpea.reAnimator.x, repeaterpea.reAnimator.y}))
        * ReAnimator::transformToSFML(repeaterpea.reAnimator.curTransforms["anim_face"])).getMatrix()[13] + 7

    };
    Bullet newBullet = { PEA,{currPOS.x,currPOS.y},repeaterpea.row };
    bullets.push(newBullet);

    repeaterpea.state = 0;
    repeaterpea.timer = GENERATE_REPEATERBULLET_TIMER;
  }
}

void drawRepeaterpea(Plant& repeaterpea) {
  //window->draw(repeaterpea.sprite);
}
