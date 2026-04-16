#include <Plants/Peashooter.hpp>
#include <Plants/Plant.hpp>
#include<Bullet.hpp>

void updatePeashooter(Plant& peashooter, float dt) {
  animateSpritesheet(peashooter.sheet, dt);

  peashooter.timer -= dt;
  if (peashooter.timer <= 0) {
    peashooter.timer = GENERATE_BULLET_TIMER;

    sf::Vector2f currPOS = peashooter.sprite.getPosition();
    Bullet newBullet = { PEA,{currPOS.x + 25.0f,currPOS.y-25.0f},peashooter.row };
    bullets.push(newBullet);
  }
}

void drawPeashooter(Plant& peashooter) {
  window->draw(peashooter.sprite);
}
