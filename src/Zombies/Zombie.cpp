#include <Zombies/Zombie.hpp>
#include <Window.hpp>
#include <BackgroundManager.hpp>

Array<Zombie> zombies[ROWS_NUMBER];

std::string Zombie::types[] = {"Regular", "Conehead", "Buckethead", "Flag", "Newspaper"};
std::string Zombie::states[] = {"zombie", "attack", "die"};

Zombie::Zombie(sf::Vector2f pos, ReAnimationDefinition *def, int row) : reAnimator(def, pos.x, pos.y, window) {
  reAnimator.setPosition(pos);
}

void Zombie::createZombie(float x, float y, Type type, int row) {
    //sf::Texture tempTexture; sf::Sprite tempSprite(tempTexture); 

    //Zombie zombie;

  /*re.animSpeedMulti = 1.5f;
  re.setTrackVisibility("anim_cone", false);
  re.setTrackVisibility("anim_bucket", false);
  re.setTrackVisibility("anim_screendoor", false);
  re.setTrackVisibility("Zombie_innerarm_screendoor_hand", false);
  re.setTrackVisibility("Zombie_innerarm_screendoor", false);
  re.setTrackVisibility("Zombie_outerarm_screendoor", false);
  re.setTrackVisibility("Zombie_duckytube", false);
  re.setTrackVisibility("Zombie_flaghand", false);
  re.setTrackVisibility("Zombie_mustache", false);
  re.setTrackVisibility("anim_tongue", false);
  re.playAnimation("anim_walk", true);*/
  Zombie *zombie = nullptr;

  switch (type) {
  case Zombie::Type::Regular:
      zombie = new Zombie({ x, y }, ReAnimator::getDefinition(REANIM_ZOMBIE_BASIC), row);
      zombie->reAnimator.setTrackVisibility("anim_cone", false);
      zombie->reAnimator.setTrackVisibility("anim_bucket", false);
      zombie->reAnimator.setTrackVisibility("anim_screendoor", false);
      zombie->reAnimator.setTrackVisibility("Zombie_innerarm_screendoor_hand", false);
      zombie->reAnimator.setTrackVisibility("Zombie_innerarm_screendoor", false);
      zombie->reAnimator.setTrackVisibility("Zombie_outerarm_screendoor", false);
      zombie->reAnimator.setTrackVisibility("Zombie_duckytube", false);
      zombie->reAnimator.setTrackVisibility("Zombie_flaghand", false);
      zombie->reAnimator.setTrackVisibility("Zombie_mustache", false);
      zombie->reAnimator.setTrackVisibility("anim_tongue", false);
      zombie->reAnimator.playAnimation("anim_walk", LoopType::Loop);
      zombie->reAnimator.animSpeedMulti = 1.5f;
      break;
  case Zombie::Type::Conehead:
    zombie = new Zombie({ x, y }, ReAnimator::getDefinition(REANIM_ZOMBIE_BASIC), row);
    zombie->reAnimator.setTrackVisibility("anim_bucket", false);
    zombie->reAnimator.setTrackVisibility("anim_screendoor", false);
    zombie->reAnimator.setTrackVisibility("Zombie_innerarm_screendoor_hand", false);
    zombie->reAnimator.setTrackVisibility("Zombie_innerarm_screendoor", false);
    zombie->reAnimator.setTrackVisibility("Zombie_outerarm_screendoor", false);
    zombie->reAnimator.setTrackVisibility("Zombie_duckytube", false);
    zombie->reAnimator.setTrackVisibility("Zombie_flaghand", false);
    zombie->reAnimator.setTrackVisibility("Zombie_mustache", false);
    zombie->reAnimator.setTrackVisibility("anim_tongue", false);
    zombie->reAnimator.setTrackVisibility("anim_hair", false);
    zombie->reAnimator.playAnimation("anim_walk", LoopType::Loop);
    zombie->reAnimator.animSpeedMulti = 1.5f;
    break;
  case Zombie::Type::Buckethead:
    zombie = new Zombie({ x, y }, ReAnimator::getDefinition(REANIM_ZOMBIE_BASIC), row);
    zombie->reAnimator.setTrackVisibility("anim_cone", false);
    zombie->reAnimator.setTrackVisibility("anim_screendoor", false);
    zombie->reAnimator.setTrackVisibility("Zombie_innerarm_screendoor_hand", false);
    zombie->reAnimator.setTrackVisibility("Zombie_innerarm_screendoor", false);
    zombie->reAnimator.setTrackVisibility("Zombie_outerarm_screendoor", false);
    zombie->reAnimator.setTrackVisibility("Zombie_duckytube", false);
    zombie->reAnimator.setTrackVisibility("Zombie_flaghand", false);
    zombie->reAnimator.setTrackVisibility("Zombie_mustache", false);
    zombie->reAnimator.setTrackVisibility("anim_tongue", false);
    zombie->reAnimator.setTrackVisibility("anim_hair", false);
    zombie->reAnimator.playAnimation("anim_walk", LoopType::Loop);
    zombie->reAnimator.animSpeedMulti = 1.5f;
    break;
  case Zombie::Type::Flag:
    zombie = new Zombie({ x, y }, ReAnimator::getDefinition(REANIM_ZOMBIE_BASIC), row);
    zombie->reAnimator.child = new ReAnimator(ReAnimator::getDefinition(REANIM_FLAGPOLE), 0, 0, window);
    zombie->reAnimator.child->hasParent = true;
    zombie->reAnimator.childsParentTrack = "Zombie_flaghand";
    zombie->reAnimator.setTrackVisibility("anim_cone", false);
    zombie->reAnimator.setTrackVisibility("anim_bucket", false);
    zombie->reAnimator.setTrackVisibility("anim_screendoor", false);
    zombie->reAnimator.setTrackVisibility("Zombie_innerarm_screendoor_hand", false);
    //zombie->reAnimator.setTrackVisibility("Zombie_innerarm_screendoor", false);
    zombie->reAnimator.setTrackVisibility("Zombie_outerarm_screendoor", false);
    zombie->reAnimator.setTrackVisibility("Zombie_duckytube", false);
    ////zombie->reAnimator.setTrackVisibility("Zombie_flaghand", true);
    zombie->reAnimator.setTrackVisibility("Zombie_mustache", false);
    zombie->reAnimator.setTrackVisibility("anim_tongue", false);
    zombie->reAnimator.setTrackVisibility("anim_innerarm1", false);
    zombie->reAnimator.setTrackVisibility("anim_innerarm2", false);
    zombie->reAnimator.setTrackVisibility("anim_innerarm3", false);
    zombie->reAnimator.playAnimation("anim_walk", LoopType::Loop);
    zombie->reAnimator.child->playAnimation("main");
    zombie->reAnimator.animSpeedMulti = 1.8f;
    break;
  }
  

  zombie->type = type;
  zombie->state = Walking;

  zombie->position = {x, y};
  //zombie->gridPosition = positionToGrid({ x, y });
  //zombie->velocity = {-speeds[type], 0};

  zombie->health = healths[type];
  zombie->strength = strengths[type];

  zombie->setSprite();

  zombies[row].push(*zombie);
}

void Zombie::setSprite() {
  if (state == Zombie::State::Attacking) {
    reAnimator.stopAnimation("anim_walk");
    reAnimator.allowMotion = false;
    if (!reAnimator.isPlayingAnimation("anim_eat")) {
      //std::cout << "Replaying eat\n";
      reAnimator.playAnimation("anim_eat", LoopType::Loop);
    }
  }
  else if (state == Zombie::State::Walking) {
    reAnimator.stopAnimation("anim_eat");
    if (!reAnimator.isPlayingAnimation("anim_walk"))
      reAnimator.playAnimation("anim_walk", LoopType::Loop);
  }
    /*std::string location = "assets/Zombies/" + types[type] + (headless ? "/headless_" : "/") + states[state] + ".png";
    sf::Texture& texture = getTexture(location);
    sprite.setTexture(texture);
    sprite.setTextureRect({{0, 0}, {166, 144}});
    sprite.setOrigin(sprite.getLocalBounds().size / 2.0f);
    int count = frameCount[type][state][headless];
    sheet = Spritesheet{nullptr, 166, 144, count, 2.f / count};*/
}

bool Zombie::update(float dt) {
  reAnimator.update(dt);
  gridPosition = positionToGrid(reAnimator.getPosition());
  onGrid = (gridPosition != sf::Vector2{-1, -1});
  checkState(dt);
  //draw(dt);
  return health > 0;
}

void Zombie::takeDamage(float damage) {
    health -= damage;
    if(health <= 0) {
        die();
        return;
    } else if(health <= 20) {
        headless = true;
    }
    if (health <= 40 && type == Zombie::Type::Flag)
      reAnimator.child->trackInstances[1].imageOverride
        = ReAnimator::getDefinition(REANIM_FLAGPOLE)->textureMap["IMAGE_REANIM_ZOMBIE_FLAG3"];
    if (health <= healths[type]*0.75f && health > healths[type]*0.5f) {
      if (type == Zombie::Type::Conehead)
        reAnimator.trackInstances[40].imageOverride
        = ReAnimator::getDefinition(REANIM_ZOMBIE_BASIC)->textureMap["IMAGE_REANIM_ZOMBIE_CONE2"];
      else if (type == Zombie::Type::Buckethead)
        reAnimator.trackInstances[41].imageOverride
        = ReAnimator::getDefinition(REANIM_ZOMBIE_BASIC)->textureMap["IMAGE_REANIM_ZOMBIE_BUCKET2"];
    }
    else if (health < healths[type]*0.5f && health >= healths[type]*0.2f) {
      if (type == Zombie::Type::Conehead)
        reAnimator.trackInstances[40].imageOverride
        = ReAnimator::getDefinition(REANIM_ZOMBIE_BASIC)->textureMap["IMAGE_REANIM_ZOMBIE_CONE3"];
      else if (type == Zombie::Type::Buckethead)
        reAnimator.trackInstances[41].imageOverride
        = ReAnimator::getDefinition(REANIM_ZOMBIE_BASIC)->textureMap["IMAGE_REANIM_ZOMBIE_BUCKET3"];
    }
    else if (health < healths[type]*0.2f) {
      if (type == Zombie::Type::Conehead)
        reAnimator.setTrackVisibility("anim_cone", false);
      else if (type == Zombie::Type::Buckethead)
        reAnimator.setTrackVisibility("anim_bucket", false);
      else if (type == Zombie::Type::Flag)
        reAnimator.child->stopAnimation("main"); // temporary
    }

    if(damage) freezeTimer = FreezeTimer;
}

void Zombie::checkState(float dt) {
  if (state == Walking) move(dt);
  else if (state == Attacking) attack(dt);
  else if (state == Dying) updateDeath(dt);
}

void Zombie::move(float dt) {
    if(onGrid && grid[gridPosition.x][gridPosition.y].plant.has_value()) {
        state = Attacking;
        setSprite();
        return;
    }
    reAnimator.allowMotion = true;
    setSprite();
    //position += velocity * dt;
    //sprite.setPosition(position);
}

void Zombie::attack(float dt) {
    if(onGrid && !grid[gridPosition.x][gridPosition.y].plant.has_value()) {
        state = Walking;
        setSprite();
        return;
    }
    attackTimer -= dt;
    if(attackTimer <= 0) {
         //hitGridCell();
      grid[gridPosition.x][gridPosition.y].plant->health -= strength;
      attackTimer = AttackTimer;
    }
    setSprite();
}

void Zombie::die() {
    state = Dying;
    //velocity = {0, 0};
    reAnimator.stopAnimation("anim_walk");
    reAnimator.stopAnimation("anim_eat");
    reAnimator.playAnimation("anim_death", LoopType::HoldLastFrame, 4.0f);
    setSprite();
}

void Zombie::updateDeath(float dt) {
  if (!reAnimator.isPlayingAnimation("anim_death")) {
    remove = true;
  }
  else {
    // will be changed later into a better option
    corpseDissapearTimer += dt;
    if (corpseDissapearTimer >= 4)
      corpseDissapearTimer = 4;
    reAnimator.setOpacity((uint8_t)(255 - (int)(254 + (0 - 254) * corpseDissapearTimer / 4.0f)));
  }
}

void Zombie::draw(float dt) {
  reAnimator.draw();
  //reAnimator.drawHitbox();                    // shows zombies hitbox as a red rectangle
  
  //sf::RectangleShape rec({2, 2});
  //rec.setPosition(reAnimator.getPosition());  // shows zombies actual x, y position as a white dot
  //window->draw(rec);

    if(freezeTimer <= 0) {

        //animateSpritesheet(sheet, dt);
    }
}


void Zombie::updateAll(float dt) {
  for (int r = 0; r < ROWS_NUMBER; r++) {
    for (int i = 0; i < zombies[r].size; i++) {
      zombies[r][i].update(dt);
    }
  }

  for (int r = 0; r < ROWS_NUMBER; r++) {
    zombies[r].erase([](Zombie& z) { return z.remove; });
  }

}

void Zombie::drawAll(float dt) {
  window->setView(*gameView);
  for (int r = 0; r < ROWS_NUMBER; r++) {
    for (int i = 0; i < zombies[r].size; i++) {
      zombies[r][i].draw(dt);
    }
  }

  /*sf::RectangleShape rec({2, 1000});
  rec.setFillColor(sf::Color(255, 0, 0, 255));
  rec.setPosition({ 1130 , 0});
  window->draw(rec);*/

  window->setView(*view);
}


bool Zombie::isZombieAliveInRow(int row, float startPosX) {
  for (int i = 0; i < zombies[row].size; i++) {
    if (zombies[row][i].health > 0 && // alive
      zombies[row][i].reAnimator.getPosition().x >= startPosX && // in front of plant
      zombies[row][i].reAnimator.getPosition().x <= 1130) // inside play area
      return true;
  }
  return false;
}
