#pragma once
#include <Array.hpp>
#include <SFML/Graphics.hpp>
#include <globals.hpp>
#include <AssetsManager.hpp>
#include <Window.hpp>

enum ParticleEffect {
  PEFF_POWIE = 0
};

struct ParticleSystem {


  struct Curve {
    float start;
    float end;
    float perc; // percentage where particle starts disappearing
  };

  struct Emitter {
    sf::Texture *tex;
    int spawnCount;
    sf::Vector2f radius;
    sf::Vector2f speed;
    sf::Vector2f duration;
    sf::Vector2f spinSpeed;
    sf::Vector2f angle = {0, 3.14159265*2};

    Curve scale;
    Curve alpha;

    float startR, endR;
    float startG, endG;
    float startB, endB;

    float frictionX;
    float frictionY;
    bool useOvershoot = false;
  };

  struct ParticleDefintion {
    Array<Emitter> emitters;
  };



  struct Particle {
    Emitter *e;
    sf::Sprite sprite;
    sf::Vector2f velocity;
    float lifeTimer;
    float maxLife;
    bool remove = false;

    Particle(Emitter *e);
  };


  static Array<Particle> particles;
  static Array<ParticleDefintion*> definitions;

  static void spawnParticleEffect(ParticleEffect effectID, sf::Vector2f pos);
  static void update(float dt);
  static void draw();
  static void init(); // load definitions

};
