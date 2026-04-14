#pragma once

#include <Array.hpp>
#include <SFML/Graphics.hpp>

const int BULLET_VELOCITY = 300;
const float PARTICLE_DEFAULT_TIMER = 0.3f;

enum BulletType { PEA, PEAICE };

struct Bullet {
  sf::Sprite sprite;
  sf::CircleShape shadow;
  int damage;
  int row;
  bool remove;
  float particleTimer;
  BulletType type;

  Bullet(BulletType type, sf::Vector2f position, float row);

  void update(float deltaTime);

  void draw();
};

extern Array<Bullet> bullets;
