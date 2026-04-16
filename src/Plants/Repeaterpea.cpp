#include <Plants/Repeaterpea.hpp>
#include <Plants/Plant.hpp>
#include<Bullet.hpp>

Plant createRepeaterpea(float x, float y, int row) {
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
}

void updateRepeaterpea(Plant& repeaterpea, float dt) {
  animateSpritesheet(repeaterpea.sheet, dt);

  repeaterpea.timer -= dt;
  if (repeaterpea.timer <= 0 && !repeaterpea.state) {
    sf::Vector2f currPOS = repeaterpea.sprite.getPosition();
    Bullet newBullet = { PEA,{currPOS.x + 25.0f,currPOS.y-25.0f},repeaterpea.row };
    bullets.push(newBullet);

    repeaterpea.state = 1;
    repeaterpea.timer = GENERATE_SECOND_REPEATERBULLET_TIMER;
  }
  if (repeaterpea.timer <= 0 && repeaterpea.state){
    sf::Vector2f currPOS = repeaterpea.sprite.getPosition();
    Bullet newBullet = { PEA,{currPOS.x + 25.0f,currPOS.y - 25.0f},repeaterpea.row };
    bullets.push(newBullet);

    repeaterpea.state = 0;
    repeaterpea.timer = GENERATE_REPEATERBULLET_TIMER;
  }
}

void drawRepeaterpea(Plant& repeaterpea) {
  window->draw(repeaterpea.sprite);
}
