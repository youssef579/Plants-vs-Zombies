#include <Plants/SnowpeaShooter.hpp>
#include <Plants/Plant.hpp>
#include<Bullet.hpp>

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
