#include <AssetsManager.hpp>
#include <Bullet.hpp>
#include <Window.hpp>
#include <Zombies/Zombie.hpp>
#include <cmath>
#include <globals.hpp>

Bullet::Bullet(BulletType typeValue, sf::Vector2f position, int rowValue)
    : sprite(typeValue == PEA
                 ? getTexture("assets/bullets/pea.png")
                 : (typeValue == SNOWPEA
                        ? getTexture("assets/bullets/peaice.png")
                        : getTexture("assets/bullets/shroom.png"))) {
  shadow.setRadius(8);
  shadow.setFillColor(sf::Color(0, 0, 0, 100));
  shadow.setScale({1.5f, 0.5f});
  shadow.setOrigin(shadow.getLocalBounds().getCenter());

  shadow.setPosition({position.x, position.y + 50});
  if(typeValue == SHROOM)
    shadow.setPosition({position.x, position.y + 20});

  sprite.setOrigin(sprite.getLocalBounds().getCenter());
  sprite.setPosition(position);

  remove = false;
  row = rowValue;
  particleTimer = INFINITY; // When set to inf, bullet is still moving
  type = typeValue;
  damage = 20;
  remove = false;

  if (type == BulletType::PEA || type == BulletType::SHROOM)
    effect = 0;
  else if (type == BulletType::SNOWPEA)
    effect = 1;
}

void Bullet::draw() {
  if (particleTimer == INFINITY) // When set to inf, bullet is still moving
    window->draw(shadow);
  window->draw(sprite);
}

void Bullet::update(float deltaTime) {
  if (particleTimer == INFINITY) { // Bullet is still moving
    float distance = BULLET_VELOCITY * deltaTime;
    sprite.move({distance, 0});
    shadow.move({distance, 0});

    // Check zombie

    for (int i = 0; i < zombies[row].size; i++) {
      if (zombies[row][i]->reAnimator.getGlobalBounds().contains(
              sprite.getPosition())) {
        if (!(zombies[row][i]->health > 0))
          continue; // skip dead zombies
        // Bullet hit zombie
        sounds.play("Splat" + std::to_string(randomRange(1, 3)));
        zombies[row][i]->takeDamage(damage, effect);
        particleTimer = PARTICLE_DEFAULT_TIMER;

        if (type == PEA) {
          sprite.setTexture(getTexture("assets/bullets/pea_particles.png"));
          sprite.setTextureRect(
              sf::IntRect({24 * randomRange(0, 3), 0}, {24, 24}));
        } else if (type == SNOWPEA) {
          sprite.setTexture(getTexture("assets/bullets/peaice_particles.png"));
          sprite.setTextureRect(
              sf::IntRect({24 * randomRange(0, 3), 0}, {24, 24}));
        } else if (type == SHROOM)
          sprite.setTexture(getTexture("assets/bullets/shroom_particles.png"));
      }
    }

    if (sprite.getPosition().x > WINDOW_SIZE.x) { // Bullet out of bounds
      remove = true;
    }
  } else { // Bullet particles
    particleTimer -= deltaTime;

    float scaleProgress = 1.0f - (particleTimer / PARTICLE_DEFAULT_TIMER);
    sprite.setScale({scaleProgress, scaleProgress});

    if (particleTimer <= 0) {
      remove = true;
    }
  }
}

void Bullet::updateAll(float dt) {
  for (int i = 0; i < bullets.size; i++)
    bullets[i].update(dt);

  bullets.erase([](const Bullet &b) { return b.remove; });
}

void Bullet::drawAll() {
  for (int i = 0; i < bullets.size; i++)
    bullets[i].draw();
}
