#include <SunFlower.hpp>
#include <SunManager.hpp>

Plant createSunFlower(float x, float y /* the way used in grid */) {
  static sf::Texture& sunFlowerTexture = getTexture("assets/sunflower_spritesheet.png");
  sf::Sprite sunFlowerSprite(sunFlowerTexture);
  sunFlowerSprite.setTextureRect({{0, 0}, {80, 80}});

  Plant newPlant = {sunFlowerSprite, 10, Plant::PlantType::SUN_FLOWER, nullptr, 10};
  Spritesheet sunFlowerSheet = Spritesheet{&newPlant.sprite, 80, 80, 24, 0.03f};
  newPlant.sheet = sunFlowerSheet;

  newPlant.sprite.setPosition({x, y});

  return newPlant;
}


void updateSunFlower(Plant &sunFlower, float dt){
  animateSpritesheet(sunFlower.sheet, dt);
  sunFlower.Timer -= dt;
  if (sunFlower.Timer <= 0){
    // handle generating suns
    sunFlower.Timer = GENERATE_SUN_FLOWER_INTERVAL;
  }else if (sunFlower.Timer <= 2){
    sunFlower.sprite.setColor(sf::Color(150, 150, 150, 150));
  }else{
    sunFlower.sprite.setColor(sf::Color(255, 255, 255));
  }
}