#include <Plants/Icepea.hpp>
#include <Plants/Plant.hpp>
#include<Bullet.hpp>

Plant createIcepea(float x, float y, int row) {
  static sf::Texture& peashooterTexture = getTexture("assets/Plants/Icepea.png");
  // peashooterTexture.setSmooth(true);

  sf::Sprite peashooterSprite(peashooterTexture);
  peashooterSprite.setTextureRect({ {0,0},{353, 368}});
  peashooterSprite.setScale({0.218, 0.217});

  Plant plant = {peashooterSprite, 100, PlantType::ICEPEA, GENERATE_ICEBULLET_TIMER, row, nullptr};
  Spritesheet peashooterSheet = Spritesheet{&plant.sprite, 353, 368, 25, 0.05f};
  plant.sheet = peashooterSheet;

  plant.sprite.setOrigin(plant.sprite.getLocalBounds().size / 2.0f);
  plant.sprite.setPosition({x, y});

  return plant;
}

void updateIcepea(Plant& peashooter, float dt) {
  animateSpritesheet(peashooter.sheet, dt);

  peashooter.timer -= dt;
  if (peashooter.timer <= 0) {
    peashooter.timer = GENERATE_ICEBULLET_TIMER;

    sf::Vector2f currPOS = peashooter.sprite.getPosition();
    Bullet newBullet = {PEAICE, {currPOS.x + 40.0f, currPOS.y - 25.0f}, peashooter.row};
    bullets.push(newBullet);
  }
}

void drawIcepea(Plant& peashooter) {
  window->draw(peashooter.sprite);
}
