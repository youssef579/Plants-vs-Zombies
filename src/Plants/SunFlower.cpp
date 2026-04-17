#include <Plants/SunFlower.hpp>
#include <SunManager.hpp>
#include <Plants/Plant.hpp>


/*Plant createSunFlower(float x, float y  the way used in grid , int row) {
  static sf::Texture& sunFlowerTexture = getTexture("assets/Plants/sunflower.png");

  sf::Sprite sunFlowerSprite(sunFlowerTexture);
  sunFlowerSprite.setTextureRect({{0, 0}, {80, 80}});

  Plant newPlant = {sunFlowerSprite, SUN_FLOWER_HEALTH, PlantType::SUN_FLOWER, 10, row,0, nullptr};

  newPlant.sprite.setOrigin(newPlant.sprite.getLocalBounds().size / 2.0f);

  newPlant.sprite.setPosition({x, y});

  return newPlant;
}*/

void updateSunFlower(Plant &sunFlower, float dt){
  //animateSpritesheet(sunFlower.sheet, dt);
  sunFlower.reAnimator.update(dt);
  //sunFlower.reAnimator.drawHitbox();


  sunFlower.timer -= dt;
  sunFlower.blinkTimer -= dt;
  if (sunFlower.timer <= 0){
    Sounds::play("sunFlowerPop");
    Sun::generate({sunFlower.reAnimator.getPosition()}, 50, 1);
    sunFlower.timer = GENERATE_SUN_FLOWER_INTERVAL;
    sunFlower.reAnimator.setOverlayAlpha(0);
  }
  if (sunFlower.blinkTimer <= 0) {
    sunFlower.reAnimator.playAnimation("blink", false);
    sunFlower.blinkTimer = randomRange(1.0f, 3.0f);
  }
}

void drawSunFlower(Plant &sunFlower){
    //window->draw(sunFlower.sprite);
  //std::cout << "timer: " << sunFlower.timer << "\n";
  //std::cout << "a: " << (int)sunFlower.reAnimator.trackInstances[27].colorOverlay.a << "\n";
  if (sunFlower.timer <= 2.5f) {
    sunFlower.reAnimator.setOverlayAlpha(1 - sunFlower.timer / 2.5f);
  }
  else if (sunFlower.timer >= 9.0f) { // glow down
    //sunFlower.reAnimator.setOverlayAlpha(sunFlower.reAnimator.trackInstances[0].colorOverlay.a - dt);
    sunFlower.reAnimator.setOverlayAlpha(1 - (10.0f - sunFlower.timer) / 1.0f);
  }
  //if (sunFlower.timer <= SHINE_TIME)
    //sunFlower.reAnimator.setOverlayAlpha();
      //window->draw(sunFlower.sprite, sf::RenderStates(sf::BlendAdd));
}
