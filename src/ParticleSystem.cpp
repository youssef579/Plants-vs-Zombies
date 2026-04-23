#include <ParticleSystem.hpp>
#include <cmath>

Array<ParticleSystem::Particle> ParticleSystem::particles;
Array<ParticleSystem::ParticleDefintion *> ParticleSystem::definitions;

ParticleSystem::Particle::Particle(Emitter *em) : e(em), sprite(*em->tex) {}

void ParticleSystem::spawnParticleEffect(ParticleEffect effectID,
                                         sf::Vector2f pos) {
  ParticleDefintion *effectDef = definitions[effectID];
  for (int i = 0; i < effectDef->emitters.size; i++) {
    Emitter *e = &effectDef->emitters[i];

    for (int j = 0; j < e->spawnCount; j++) {
      Particle p(e);

      p.sprite.setOrigin((sf::Vector2f)e->tex->getSize() / 2.0f);

      float angle = randomRange(e->angle.x, e->angle.y);
      float radius = randomRange(e->radius.x, e->radius.y);
      // dx = startX + R*Cos(theta), dy =
      p.sprite.setPosition(
          {pos.x + std::cos(angle) * radius, pos.y + std::sin(angle) * radius});

      float speed = randomRange(e->speed.x, e->speed.y);
      p.velocity =
          sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);

      p.maxLife = randomRange(e->duration.x, e->duration.y);
      p.lifeTimer = 0.0f;
      p.sprite.setRotation(sf::radians(randomRange(e->angle.x, e->angle.y)));

      particles.push(p);
    }
  }
}

void ParticleSystem::update(float dt) {
  for (int i = 0; i < particles.size; i++) {
    Particle &p = particles[i];
    Emitter *e = p.e;

    p.lifeTimer += dt;
    float progress = p.lifeTimer / p.maxLife; // 0 to 1

    if (progress >= 1.0f) {
      p.remove = true;
      continue;
    }

    p.velocity.x -= p.velocity.x * e->frictionX * dt;
    p.velocity.y -= p.velocity.y * e->frictionY * dt;

    p.sprite.move(p.velocity * dt);
    p.sprite.rotate(sf::degrees((e->spinSpeed.x + e->spinSpeed.y) / 2.0f) * dt);

    // Scale Curve
    float curScale = e->scale.start;
    if (progress > e->scale.perc) {
      float lerpT = (progress - e->scale.perc) / (1.0f - e->scale.perc);
      curScale = e->scale.start + (e->scale.end - e->scale.start) * lerpT;
    }
    p.sprite.setScale({curScale, curScale});

    // Alpha Curve
    float curAlpha = e->alpha.start;
    if (progress > e->alpha.perc) {
      float lerpT = (progress - e->alpha.perc) / (1.0f - e->alpha.perc);
      curAlpha = e->alpha.start + (e->alpha.end - e->alpha.start) * lerpT;
    }

    // Color Curves
    float curR = e->startR + (e->endR - e->startR) * progress;
    float curG = e->startG + (e->endG - e->startG) * progress;
    float curB = e->startB + (e->endB - e->startB) * progress;

    p.sprite.setColor(
        sf::Color((uint8_t)(curR * 255.0f), (uint8_t)(curG * 255.0f),
                  (uint8_t)(curB * 255.0f), (uint8_t)(curAlpha * 255.0f)));
  }

  particles.erase([](Particle &p) { return p.remove; });
}

void ParticleSystem::init() {

  ParticleDefintion *def = new ParticleDefintion;

  ParticleSystem::Emitter powieText;
  powieText.tex = &getTexture("assets/Particles/ExplosionPowie.png");
  powieText.spawnCount = 1;
  powieText.radius = {0.0f, 0.0f};

  // powieText.angle = { -1.5707f, -1.5707f };
  powieText.angle = {-1.5707f / 2.0f, 1.5707f / 2.0f};
  powieText.speed = {40.0f, 40.0f};
  powieText.duration = {1.0f, 1.0f};
  powieText.spinSpeed = {0.0f, 0.0f};

  powieText.scale = {1.0f, 1.0f, 1.0f};
  powieText.alpha = {1.0f, 0.0f, 0.75f};
  // powieText.startG = 1.0f;
  // powieText.endG = 1.0f;
  powieText.frictionX = 0.0f;
  powieText.frictionY = 0.0f;
  powieText.startR = 1.0f;
  powieText.endR = 1.0f;
  powieText.startG = 1.0f;
  powieText.endG = 1.0f;
  powieText.startB = 1.0f;
  powieText.endB = 1.0f;

  ParticleSystem::Emitter smallClouds;
  smallClouds.tex = &getTexture("assets/Particles/ExplosionCloud.png");
  smallClouds.spawnCount = 14;
  smallClouds.radius = {10.0f, 50.0f};
  smallClouds.speed = {1000.0f, 1250.0f};
  smallClouds.duration = {0.4f, 0.6f};
  smallClouds.spinSpeed = {-200.0f, 200.0f};
  smallClouds.scale = {0.5f, 0.0f, 0.60f};
  smallClouds.alpha = {1.0f, 0.0f, 0.70f};
  // smallClouds.startG = 0.7f;
  // smallClouds.endG = 0.0f;
  smallClouds.frictionX = 8.0f;
  smallClouds.frictionY = 8.0f;
  smallClouds.startR = 1.0f;
  smallClouds.endR = 1.0f;
  smallClouds.startG = 0.7f;
  smallClouds.endG = 0.0f;
  smallClouds.startB = 0.0f;
  smallClouds.endB = 0.0f;

  ParticleSystem::Emitter bigClouds;
  bigClouds.tex = &getTexture("assets/Particles/ExplosionCloud.png");
  ;
  bigClouds.spawnCount = 14;
  bigClouds.radius = {10.0f, 50.0f};
  bigClouds.speed = {250.0f, 600.0f};
  bigClouds.duration = {0.4f, 0.6f};
  bigClouds.spinSpeed = {-200.0f, 200.0f};
  bigClouds.scale = {1.4f, 0.0f, 0.60f};
  bigClouds.alpha = {1.0f, 0.0f, 0.70f};
  // bigClouds.startG = 0.9f;
  // bigClouds.endG = 0.4f;
  bigClouds.frictionX = 8.0f;
  bigClouds.frictionY = 8.0f;
  bigClouds.startR = 1.0f;
  bigClouds.endR = 1.0f;
  bigClouds.startG = 0.9f;
  bigClouds.endG = 0.4f;
  bigClouds.startB = 0.0f;
  bigClouds.endB = 0.0f;

  def->emitters.push(powieText);
  def->emitters.push(smallClouds);
  def->emitters.push(bigClouds);
  definitions.push(def);
}

void ParticleSystem::draw() {
  sf::RenderStates states;
  states.blendMode = sf::BlendAdd;

  for (int i = 0; i < particles.size; i++) {
    window->draw(particles[i].sprite, states);
  }
}
