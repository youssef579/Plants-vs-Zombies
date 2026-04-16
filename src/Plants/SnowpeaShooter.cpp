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
  animateSpritesheet(snowpeaShooter.sheet, dt);

  snowpeaShooter.timer -= dt;
  if (snowpeaShooter.timer <= 0) {
    snowpeaShooter.timer = GENERATE_SNOWBULLET_TIMER;

    sf::Vector2f currPOS = snowpeaShooter.sprite.getPosition();
    Bullet newBullet = {SNOWPEA, {currPOS.x + 25, currPOS.y - 25.0f}, snowpeaShooter.row};
    bullets.push(newBullet);
  }
}

void drawSnowpeaShooter(Plant& snowpeaShooter) {
  window->draw(snowpeaShooter.sprite);
}
