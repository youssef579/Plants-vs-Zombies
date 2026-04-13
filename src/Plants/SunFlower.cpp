#include <Plants/SunFlower.hpp>
#include <SunManager.hpp>
#include <Plants/Plant.hpp>


Plant createSunFlower(float x, float y /* the way used in grid */, int row) {
  static sf::Texture& sunFlowerTexture = getTexture("assets/Plants/sunflower.png");
  sunFlowerTexture.setSmooth(true);

  sf::Sprite sunFlowerSprite(sunFlowerTexture);
  sunFlowerSprite.setTextureRect({{0, 0}, {80, 80}});

  Plant newPlant = {sunFlowerSprite, SUN_FLOWER_HEALTH, PlantType::SUN_FLOWER, 10, row, nullptr};
  Spritesheet sunFlowerSheet = Spritesheet{&newPlant.sprite, 80, 80, 24, 0.05f};
  newPlant.sheet = sunFlowerSheet;

  newPlant.sprite.setOrigin(newPlant.sprite.getLocalBounds().size / 2.0f);

  newPlant.sprite.setPosition({x, y});

  return newPlant;
}

void updateSunFlower(Plant &sunFlower, float dt){
  animateSpritesheet(sunFlower.sheet, dt);
  sunFlower.timer -= dt;
  if (sunFlower.timer <= 0){
    Sounds::play("sunFlowerPop");
    Sun::generate(sunFlower.sprite.getPosition().x, sunFlower.sprite.getPosition().y, 50, 1);
    sunFlower.timer = GENERATE_SUN_FLOWER_INTERVAL;
  }
}

void drawSunFlower(Plant &sunFlower){
    window->draw(sunFlower.sprite);
    if (sunFlower.timer <= SHINE_TIME)
      window->draw(sunFlower.sprite, sf::RenderStates(sf::BlendAdd));
}
