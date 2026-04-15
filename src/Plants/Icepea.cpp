#include <Plants/Icepea.hpp>
#include <Plants/Plant.hpp>
#include<Bullet.hpp>

Plant createIcepea(float x, float y, int row) {
  static sf::Texture& icepeaTexture = getTexture("assets/Plants/Icepea.png");
  // icepeaTexture.setSmooth(true);

  sf::Sprite icepeaSprite(icepeaTexture);
  icepeaSprite.setTextureRect({ {0,0},{353, 368}});
  icepeaSprite.setScale({0.218, 0.217});

  Plant plant = {icepeaSprite, 100, PlantType::ICEPEA, GENERATE_ICEBULLET_TIMER, row, nullptr};
  Spritesheet icepeaSheet = Spritesheet{&plant.sprite, 353, 368, 25, 0.05f};
  plant.sheet = icepeaSheet;

  plant.sprite.setOrigin(plant.sprite.getLocalBounds().size / 2.0f);
  plant.sprite.setPosition({x, y});

  return plant;
}

void updateIcepea(Plant& icepea, float dt) {
  animateSpritesheet(icepea.sheet, dt);

  icepea.timer -= dt;
  if (icepea.timer <= 0) {
    icepea.timer = GENERATE_ICEBULLET_TIMER;

    sf::Vector2f currPOS = icepea.sprite.getPosition();
    Bullet newBullet = {PEAICE, {currPOS.x + 25, currPOS.y - 25.0f}, icepea.row};
    bullets.push(newBullet);
  }
}

void drawIcepea(Plant& icepea) {
  window->draw(icepea.sprite);
}
