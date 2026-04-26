#include <Zombies/Zombie.hpp>
#include <Window.hpp>
#include <BackgroundManager.hpp>
#include <LawnMower.hpp>

Array<Zombie> zombies[ROWS_NUMBER];

std::string Zombie::types[] = {"Regular", "Conehead", "Buckethead", "Flag", "Newspaper"};
std::string Zombie::states[] = {"zombie", "attack", "die"};

sf::SoundBuffer Zombie::soundBuffer_zombieBite;
sf::SoundBuffer Zombie::soundBuffer_zombieGulp;

Zombie::Zombie(sf::Vector2f pos, ReAnimationDefinition *def, int row)
  : reAnimator(def, pos.x, pos.y, window),
  sound_zombieBite(soundBuffer_zombieBite),
  sound_zombieGulp(soundBuffer_zombieGulp){
    reAnimator.setPosition(pos);
}

void Zombie::init() {
  soundBuffer_zombieBite = getSoundBuffer("assets/sounds/sfx_zombieBite.ogg");
  soundBuffer_zombieGulp = getSoundBuffer("assets/sounds/sfx_zombieGulp.ogg");
}

void Zombie::createZombie(float x, float y, Type type, int ROW, float startDel) {
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
      zombie = new Zombie({ x, y }, ReAnimator::getDefinition(REANIM_ZOMBIE_BASIC), ROW);
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
      //zombie->reAnimator.playAnimation("anim_walk", LoopType::Loop);
      zombie->reAnimator.playAnimation("anim_idle", LoopType::Loop);
      //zombie->reAnimator.animSpeedMulti = 1.5f;
      break;
  case Zombie::Type::Conehead:
    zombie = new Zombie({ x, y }, ReAnimator::getDefinition(REANIM_ZOMBIE_BASIC), ROW);
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
    //zombie->reAnimator.playAnimation("anim_walk", LoopType::Loop);
    zombie->reAnimator.playAnimation("anim_idle", LoopType::Loop);
    //zombie->reAnimator.animSpeedMulti = 1.5f;
    break;
  case Zombie::Type::Buckethead:
    zombie = new Zombie({ x, y }, ReAnimator::getDefinition(REANIM_ZOMBIE_BASIC), ROW);
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
    //zombie->reAnimator.playAnimation("anim_walk", LoopType::Loop);
    zombie->reAnimator.playAnimation("anim_idle", LoopType::Loop);
    //zombie->reAnimator.animSpeedMulti = 1.5f;
    break;
  case Zombie::Type::Flag:
    zombie = new Zombie({ x, y }, ReAnimator::getDefinition(REANIM_ZOMBIE_BASIC), ROW);
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
    //zombie->reAnimator.playAnimation("anim_walk", LoopType::Loop);
    zombie->reAnimator.playAnimation("anim_idle", LoopType::Loop);
    zombie->reAnimator.child->playAnimation("main");
    //zombie->reAnimator.animSpeedMulti = 1.8f;
    break;
  case Zombie::Type::Screendoor:
    zombie = new Zombie({ x, y }, ReAnimator::getDefinition(REANIM_ZOMBIE_BASIC), ROW);
    zombie->reAnimator.setTrackVisibility("anim_cone", false);
    zombie->reAnimator.setTrackVisibility("anim_bucket", false);
    //zombie->reAnimator.setTrackVisibility("anim_screendoor", false);
    //zombie->reAnimator.setTrackVisibility("Zombie_innerarm_screendoor_hand", false);
    //zombie->reAnimator.setTrackVisibility("Zombie_innerarm_screendoor", false);
    //zombie->reAnimator.setTrackVisibility("Zombie_outerarm_screendoor", false);
    zombie->reAnimator.setTrackVisibility("Zombie_duckytube", false);
    zombie->reAnimator.setTrackVisibility("Zombie_flaghand", false);
    zombie->reAnimator.setTrackVisibility("Zombie_mustache", false);
    zombie->reAnimator.setTrackVisibility("anim_tongue", false);
    zombie->reAnimator.setTrackVisibility("Zombie_outerarm_upper", false);
    zombie->reAnimator.setTrackVisibility("Zombie_outerarm_lower", false);
    zombie->reAnimator.setTrackVisibility("Zombie_outerarm_hand", false);
    zombie->reAnimator.setTrackVisibility("anim_innerarm3", false);
    zombie->reAnimator.setTrackVisibility("anim_innerarm2", false);
    zombie->reAnimator.setTrackVisibility("anim_innerarm1", false);
    //zombie->reAnimator.playAnimation("anim_walk", LoopType::Loop);
    zombie->reAnimator.playAnimation("anim_idle", LoopType::Loop);
    break;
  }
  
  zombie->reAnimator.animSpeedMulti = speeds[type];
  for (int i = 0; i < zombie->reAnimator.trackInstances.size(); i++)
    zombie->reAnimator.trackInstances[i].colorOverlay = {0, 0, 255, 0};
  zombie->type = type;
  zombie->row = ROW;
  zombie->state = Idle;
  zombie->startDelay = startDel;
  zombie->position = {x, y};
  //zombie->gridPosition = positionToGrid({ x, y });
  //zombie->velocity = {-speeds[type], 0};

  zombie->health = healths[type];
  zombie->strength = strengths[type];

  zombie->setSprite();

  zombies[ROW].push(*zombie);
}

void Zombie::setSprite() {
  if (state == Zombie::State::Attacking) {
    if(sound_zombieBite.getStatus() != sf::Sound::Status::Playing)
      sound_zombieBite.play();
    reAnimator.stopAnimation("anim_walk");
    reAnimator.allowMotion = false;
    if (!reAnimator.isPlayingAnimation("anim_eat")) {
      //std::cout << "Replaying eat\n";
      reAnimator.playAnimation("anim_eat", LoopType::Loop);
    }
  }
  else if (state == Zombie::State::Walking) {
    if (sound_zombieBite.getStatus() == sf::Sound::Status::Playing)
      sound_zombieBite.stop();
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

  //reAnimator.setOverlayAlpha(std::max(0.0f, std::min(freezeTimer*255, 255.0f)));
  //std::cout << "set alpha to " << std::max(0.0f, std::min(freezeTimer*255, 255.0f)) << "\n";
  //std::cout << "freeze timer: " << freezeTimer << "\n";
  /*if (freezeTimer > 0) {
    reAnimator.globalColor = sf::Color(140, 140, 255);
    reAnimator.setOverlayAlpha(1.0f);
  }
  else {
    reAnimator.globalColor = sf::Color::White;
    reAnimator.setOverlayAlpha(0.0f);
  }*/

  
  float lerpFactor;
  if (freezeTimer > FreezeTimer - 0.1)
    lerpFactor = (ReAnimator::lerp(1, 0, (freezeTimer - (FreezeTimer - 0.1)) / (FreezeTimer - (FreezeTimer - 0.1))));
  else if (freezeTimer < 0.1)
    lerpFactor = (ReAnimator::lerp(1, 0, (0.1 - freezeTimer) / 0.1));
  else
    lerpFactor = (1.0f);

  reAnimator.globalColor = sf::Color(lerpFactor * 140 + (1 - lerpFactor) * 255, lerpFactor * 140 + (1 - lerpFactor) * 255, 255);
  reAnimator.setOverlayAlpha(lerpFactor);

  //std::cout << "a: " << (int)reAnimator.trackInstances[0].colorOverlay.a << "\n";

}

bool Zombie::update(float dt) {

  if (inPlayArea || reAnimator.getPosition().x <= 1130)
    inPlayArea = true;
  if (reAnimator.getPosition().x <= 150) // activate lawnmower
    LawnMower::activateLawnMower(positionToGrid({500, reAnimator.getPosition().y}).x);
  if (reAnimator.getPosition().x <= 100)
    dayLevel.playGameOverScreen(row);
    //std::system("pause");

  if (freezeTimer > 0) {
    freezeTimer -= dt;
    if (freezeTimer > 0) {
      reAnimator.animSpeedMulti = speeds[type] * 0.5f;
    }
    else {
      freezeTimer = 0;
      reAnimator.animSpeedMulti = speeds[type];
    }
  }

  if (startDelay > 0)
    startDelay -= dt;
  else if (state == Idle)
    state = Walking;

  reAnimator.update(dt);
  gridPosition = positionToGrid(reAnimator.getPosition());
  onGrid = (gridPosition != sf::Vector2{-1, -1});
  checkState(dt);
  //draw(dt);
  return health > 0;
}
//effects: 0 -> normal, 1 -> freeze, 2 -> explosion/fire , 3 -> mowed
void Zombie::takeDamage(float damage, int effect) { // todo: change effect into enum
    health -= damage;
    if (effect == 1 && !(type == Zombie::Type::Screendoor && health >= healths[type] * 0.2f)) {
      freezeTimer = FreezeTimer;
    }
    if(health <= 0) {
      /*if (effect == 2)
        die(2);
      else
        die();*/
      die(effect);
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
      else if (type == Zombie::Type::Screendoor)
        reAnimator.trackInstances[32].imageOverride
        = ReAnimator::getDefinition(REANIM_ZOMBIE_BASIC)->textureMap["IMAGE_REANIM_ZOMBIE_SCREENDOOR2"];
    }
    else if (health < healths[type]*0.5f && health >= healths[type]*0.2f) {
      if (type == Zombie::Type::Conehead)
        reAnimator.trackInstances[40].imageOverride
        = ReAnimator::getDefinition(REANIM_ZOMBIE_BASIC)->textureMap["IMAGE_REANIM_ZOMBIE_CONE3"];
      else if (type == Zombie::Type::Buckethead)
        reAnimator.trackInstances[41].imageOverride
        = ReAnimator::getDefinition(REANIM_ZOMBIE_BASIC)->textureMap["IMAGE_REANIM_ZOMBIE_BUCKET3"];
      else if (type == Zombie::Type::Screendoor)
        reAnimator.trackInstances[32].imageOverride
        = ReAnimator::getDefinition(REANIM_ZOMBIE_BASIC)->textureMap["IMAGE_REANIM_ZOMBIE_SCREENDOOR3"];
    }
    else if (health < healths[type]*0.2f) {
      if (type == Zombie::Type::Conehead)
        reAnimator.setTrackVisibility("anim_cone", false);
      else if (type == Zombie::Type::Buckethead)
        reAnimator.setTrackVisibility("anim_bucket", false);
      else if (type == Zombie::Type::Flag)
        reAnimator.child->stopAnimation("main"); // temporary
      else if (type == Zombie::Type::Screendoor) {
        reAnimator.setTrackVisibility("Zombie_outerarm_upper", true);
        reAnimator.setTrackVisibility("Zombie_outerarm_lower", true);
        reAnimator.setTrackVisibility("Zombie_outerarm_hand", true);
        reAnimator.setTrackVisibility("anim_innerarm3", true);
        reAnimator.setTrackVisibility("anim_innerarm2", true);
        reAnimator.setTrackVisibility("anim_innerarm1", true);
        reAnimator.setTrackVisibility("anim_screendoor", false);
        reAnimator.setTrackVisibility("Zombie_innerarm_screendoor_hand", false);
        reAnimator.setTrackVisibility("Zombie_innerarm_screendoor", false);
        reAnimator.setTrackVisibility("Zombie_outerarm_screendoor", false);
      }
    }

    //if(damage) freezeTimer = FreezeTimer;
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
      if (grid[gridPosition.x][gridPosition.y].plant->health <= 0)
        sound_zombieGulp.play();
      attackTimer = AttackTimer;
    }
    setSprite();
}

void Zombie::die(int effect) {
    state = Dying;
    //velocity = {0, 0};
    reAnimator.stopAnimation("anim_walk");
    reAnimator.stopAnimation("anim_eat");
    reAnimator.stopAnimation("anim_idle");
    if(effect == 0 || effect == 1)
      reAnimator.playAnimation("anim_death", LoopType::HoldLastFrame, 4.0f);
    else if (effect == 2) {
      reAnimator.switchDefinition(REANIM_ZOMBIE_CHARRED);
      reAnimator.playAnimation("anim_crumble", LoopType::HoldLastFrame, 4.0f);
      deathCause = 1;
    }
    else if (effect == 3) {
      ReAnimator::orphanAnimators.push({ ReAnimator::getDefinition(REANIM_LAWNMOWERED_ZOMBIE) , reAnimator.getPosition().x , reAnimator.getPosition().y, window});
      ReAnimator::orphanAnimators[ReAnimator::orphanAnimators.size - 1].child = &reAnimator;
      ReAnimator::orphanAnimators[ReAnimator::orphanAnimators.size - 1].childsParentTrack = "locator";
      reAnimator.hasParent = true;
      ReAnimator::orphanAnimators[ReAnimator::orphanAnimators.size - 1].playAnimation("main", HoldLastFrame, 2.0f);
      reAnimator.playAnimation("anim_idle");

      corpseDissapearTimer = 2.5f;
      deathCause = 2;
    }
    setSprite();
}

void Zombie::updateDeath(float dt) {
  if (deathCause == 0 && !reAnimator.isPlayingAnimation("anim_death")
    || deathCause == 1 && !reAnimator.isPlayingAnimation("anim_crumble")
    || deathCause == 2 && corpseDissapearTimer >= 6) {
    remove = true;
    if (reAnimator.child)
      delete reAnimator.child;
  }
  else {
    // will be changed later into a better option
    // 0 < timer < 2 -> do nothing
    // 2 <= timer < 6 -> fade out
    corpseDissapearTimer += dt * speeds[type] * (freezeTimer > 0 ? 0.5f : 1.0f);
    if (corpseDissapearTimer >= 6)
      corpseDissapearTimer = 6;
    //std::cout << "set opacity to " << (int)(uint8_t)(255 - (int)(254 + (0 - 254) * std::max(corpseDissapearTimer - 2, 0.0f) / 4.0f)) << "\n";
    reAnimator.setOpacity((uint8_t)(255 - (int)(254 + (0 - 254) * std::max(corpseDissapearTimer-2, 0.0f) / 4.0f)));
  }
}

void Zombie::draw() {
  if(deathCause != 2) // if it is mowed the orphaned locator will draw it
    reAnimator.draw();
  //reAnimator.drawHitbox();                    // shows zombies hitbox as a red rectangle
  
  //sf::RectangleShape rec({2, 2});
  //rec.setPosition(reAnimator.getPosition());  // shows zombies actual x, y position as a white dot
  //window->draw(rec);

    //if(freezeTimer <= 0) {

    //    //animateSpritesheet(sheet, dt);
    //}
}


void Zombie::updateAll(float dt) {
  for (int r = 0; r < ROWS_NUMBER; r++) {
    for (int i = 0; i < zombies[r].size; i++) {
      zombies[r][i].update(dt);
    }
  }

  for (int r = 0; r < ROWS_NUMBER; r++) {
    zombies[r].erase([](Zombie &z) { return z.remove; });
  }

}

void Zombie::drawAll() {
  window->setView(*gameView);
  for (int r = 0; r < ROWS_NUMBER; r++) {
    for (int i = 0; i < zombies[r].size; i++) {
      zombies[r][i].draw();
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
      zombies[row][i].inPlayArea) // inside play area
      return true;
  }
  return false;
}


void Zombie::updateVolumes() {
  for (int r = 0; r < ROWS_NUMBER; r++) {
    for (int i = 0; i < zombies[r].size; i++) {
      zombies[r][i].sound_zombieBite.setVolume(settings.soundFXVolume);
      zombies[r][i].sound_zombieGulp.setVolume(settings.soundFXVolume);
    }
  }
}
