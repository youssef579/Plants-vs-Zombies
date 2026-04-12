#include<bullet.hpp>
#include<SFML/Graphics.hpp>
#include<AssetsManager.hpp>
#include<window.hpp>


Bullet::Bullet(bulletType type, sf::Vector2f startPos, int row) {

  shadow.setRadius(8.f);
  shadow.setFillColor(sf::Color(0, 0, 0, 130));
  shadow.setScale({1.5f, 0.5f});
  shadow.setOrigin({ shadow.getLocalBounds().size.x / 2.f, shadow.getLocalBounds().size.y / 2.f });

  sf::FloatRect bounds = bulletSprite.getLocalBounds();
  bulletSprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
  bulletSprite.setPosition(startPos);
}

void Bullet::draw() {
  window->draw(this->shadow);
  if(!isColloid)
    window->draw(this->bulletSprite);
  if(isColloid)
    window->draw(this->particleSprite);
}

void Bullet::update(float deltaTime) {

  if (bulletSprite.getPosition().x > 500.f) {
    isColloid = true;
  }

  if (isColloid) {

      sf::FloatRect bounds = particleSprite.getLocalBounds();
      particleSprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });

    if (particleTimer > 0) {
      particleTimer -= deltaTime;
      float scaleProgress = 1.0f - (particleTimer / PARTICLE_DEFAULT_TIMER);

      particleSprite.setScale({ scaleProgress, scaleProgress });

      if (particleTimer <= 0) {
        remove = true;
      }
    }
    return;
  }

  float movedDistance = deltaTime * BULLET_VELOCITY;

  particleSprite.move({ movedDistance, 0.f });
  bulletSprite.move({ movedDistance, 0.f });
  shadow.move({ movedDistance, 0.f });

  if (bulletSprite.getPosition().x > WINDOW_SIZE.x) {
    remove = true;
  }
}
