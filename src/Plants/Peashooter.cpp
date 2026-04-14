#include <Plants/Peashooter.hpp>
#include <Plants/Plant.hpp>
#include<Bullet.hpp>

Plant createPeashooter(float x, float y, int row) {
  static sf::Texture& peashooterTexture = getTexture("assets/Plants/peashooter.png");
  // peashooterTexture.setSmooth(true);

  sf::Sprite peashooterSprite(peashooterTexture);
  peashooterSprite.setTextureRect({ {0,0},{348,359}});
  peashooterSprite.setScale({0.225, 0.225});

  Plant plant = { peashooterSprite,100,PlantType::PEASHOOTER,1.5f,row,nullptr };
  Spritesheet peashooterSheet = Spritesheet{ &plant.sprite,348,359,29,0.065f };
  plant.sheet = peashooterSheet;

  plant.sprite.setOrigin(plant.sprite.getLocalBounds().size / 2.0f);
  plant.sprite.setPosition({ x,y });

  return plant;
}

void updatePeashooter(Plant& peashooter, float dt) {
  animateSpritesheet(peashooter.sheet, dt);

  peashooter.timer -= dt;
  if (peashooter.timer <= 0) {
    peashooter.timer = GENERATE_BULLET_TIMER;

    sf::Vector2f currPOS = peashooter.sprite.getPosition();
    Bullet newBullet = { PEA,{currPOS.x+40.0f,currPOS.y-25.0f},peashooter.row };
    bullets.push(newBullet);
  }
}

void drawPeashooter(Plant& peashooter) {
  window->draw(peashooter.sprite);
}
