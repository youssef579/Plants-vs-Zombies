#pragma once
#include<Array.hpp>
#include<SFML/Graphics.hpp>

const int BULLET_VELOCITY = 200;
const float PARTICLE_DEFAULT_TIMER = 1.f;

enum bulletType{PEA, PEAICE};

struct Bullet {
  sf::Sprite bulletSprite;
  sf::Sprite particleSprite;
  sf::CircleShape shadow;

  int damage;
  int row;
  bool remove;
  bool isColloid;
  float particleTimer;

  Bullet(bulletType type, sf::Vector2f startPos, int row);
  void update(float deltaTime);
  void draw();
};

//extern Array<Bullet> bullets;
