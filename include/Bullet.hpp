#pragma once

#include <Array.hpp>
#include <SFML/Graphics.hpp>

const float BULLET_VELOCITY = 300;
const float PARTICLE_DEFAULT_TIMER = 0.3f;

enum BulletType { PEA, SNOWPEA, SHROOM };

struct Bullet {
  sf::Sprite sprite;
  sf::CircleShape shadow;
  int damage;
  int row;
  bool remove;
  float particleTimer;
  BulletType type;
  int effect;

  Bullet(BulletType type, sf::Vector2f position, int row);

  void update(float deltaTime);

  void draw();

  static void updateAll(float dt);
  static void drawAll();
};

extern Array<Bullet> bullets;
