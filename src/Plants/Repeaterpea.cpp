#include <Plants/Repeaterpea.hpp>
#include <Plants/Plant.hpp>
#include<Bullet.hpp>

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
