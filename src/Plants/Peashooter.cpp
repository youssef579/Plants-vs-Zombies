#include <Plants/Peashooter.hpp>
#include <Plants/Plant.hpp>

Plant createPeashooter(float x, float y) {
  static sf::Texture& peashooterTexture = getTexture("assets/Plants/peashooter2.png");
  peashooterTexture.setSmooth(true);

  sf::Sprite peashooterSprite(peashooterTexture);
  peashooterSprite.setTextureRect({ {0,0},{71,71} });

  Plant plant = { peashooterSprite,100,PlantType::PEASHOOTER,1.5f,nullptr };
  Spritesheet peashooterSheet = Spritesheet{ &plant.sprite,71,71,13,0.0725f };
  plant.sheet = peashooterSheet;

  plant.sprite.setOrigin(plant.sprite.getLocalBounds().size / 2.0f);
  plant.sprite.setPosition({ x,y });

  return plant;
}

void updatePeashooter(Plant& peashooter, float dt) {
  animateSpritesheet(peashooter.sheet, dt);

  peashooter.timer -= dt;
  if (peashooter.timer <= 0) {
    peashooter.timer = 1.5f;
  }
}

void drawPeashooter(Plant& peashooter) {
  window->draw(peashooter.sprite);
}
