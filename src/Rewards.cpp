#include <Rewards.hpp>
#include <Zombies/Zombie.hpp>
#include <LevelManager.hpp>

sf::Texture RewardManager::packetTextures[9] = {};
Array<RewardManager::Reward> RewardManager::rewards;
RewardManager::CollectedPacket RewardManager::collectedPacket;
bool RewardManager::isPacketCollected = false;
bool RewardManager::spawnedLevelReward = false;


void RewardManager::init() {
  packetTextures[SUN_FLOWER]       = getTexture("assets/packets/sunflower.png");
  packetTextures[WALLNUT]          = getTexture("assets/packets/wallnut.png");
  packetTextures[SNOWPEASHOOTER]   = getTexture("assets/packets/peaice.png");
  packetTextures[REPEATERPEA]      = getTexture("assets/packets/repeated.png");
  packetTextures[TALLNUT]          = getTexture("assets/packets/tallnut.png");
  packetTextures[CHERRYBOMB]       = getTexture("assets/packets/cherrybomb.png");
  packetTextures[JALAPENO]         = getTexture("assets/packets/jalapeno.png");
  packetTextures[POTATOMINE]       = getTexture("assets/packets/potatomine.png");
}


//void RewardManager::spawnReward(sf::Vector2f pos, sf::Vector2f vInitial, Reward::RewardType type) {
  //rewards.push({}); // LATER
//}

void RewardManager::spawnReward(sf::Vector2f pos, sf::Vector2f vInitial, PlantType type) {
  rewards.push({RewardManager::Reward::RewardType::Packet,
    type, pos, vInitial, {0, 600} , pos.y-30.0f, sf::Sprite(packetTextures[type])});
  rewards[rewards.size - 1].sprite.setOrigin(rewards[rewards.size - 1].sprite.getLocalBounds().size / 2.0f);
}


void RewardManager::update(float dt) {

  //std::cout << levelManager.spawningFinished << " && " << (Zombie::totalZombies == 0) << " && " << !spawnedLevelReward << "\n";
  // check to Spawn the level reward
  if (levelManager.spawningFinished && Zombie::totalZombies == 0 && !spawnedLevelReward) {
    spawnReward(Zombie::lastZombieDeathPosition, { 0, -400 }, levelManager.levels[levelManager.currentLevel-1]->reward);
    spawnedLevelReward = true;
  }

  for (int i = 0; i < rewards.size; i++)
    rewards[i].update(dt);
  if(isPacketCollected) collectedPacket.update(dt);

  rewards.erase([](Reward &r) {return r.remove; });
}

void RewardManager::draw() {
  for (int i = 0; i < rewards.size; i++)
    rewards[i].draw();
  if(isPacketCollected) collectedPacket.draw();
}




void RewardManager::Reward::update(float dt) {
  // check click LATER
  /*std::cout << "-------------\n";
  std::cout << "pos: " << position.x << ", " << position.y << "\n";
  std::cout << "vel: " << velocity.x << ", " << velocity.y << "\n";
  std::cout << "acc: " << acceleration.x << ", " << acceleration.y << "\n";
  std::cout << "gY: " << groundY << "\n";*/

  if (!(position.y >= groundY && velocity.y > 0))
    velocity = velocity + acceleration * dt; // V = Vo + at
  else {
    acceleration = { 0, 0 };
    velocity = { 0, 0 };
  }

  position += velocity * dt;
  sprite.setPosition(position);

  onClick(sprite, collectPacket, []() {});

}


void RewardManager::Reward::draw() {
  window->draw(sprite);
}


void RewardManager::collectPacket() {
  isPacketCollected = true;
  Reward *packReward;
  for (int i = 0; i < rewards.size; i++)
    if (rewards[i].type == Reward::Packet)
      packReward = &rewards[i];

  if (!packReward) return;

  collectedPacket.startPosition = packReward->position;
  collectedPacket.endPosition = (sf::Vector2f)(WINDOW_SIZE / 2u);
  collectedPacket.sprite = new sf::Sprite(packReward->sprite);
  collectedPacket.timer = 0;
  collectedPacket.whiteOverlay.setSize((sf::Vector2f)WINDOW_SIZE);
  collectedPacket.whiteOverlay.setFillColor(sf::Color(255, 255, 255, 0));

  packReward->remove = true; // delete old reward

}

void RewardManager::CollectedPacket::update(float dt) {
  collectedPacket.timer += dt;
  //if (collectedPacket.timer > collectedPacket.duration)
    //collectedPacket.timer = collectedPacket.duration;

  float lerpF = (std::min(collectedPacket.timer, collectedPacket.duration) / collectedPacket.duration);
  lerpF = lerpF * lerpF;

  collectedPacket.sprite->setPosition( // newPos = start + (end-start) * progress
    startPosition + (endPosition - startPosition) * lerpF);

  collectedPacket.sprite->setScale(
    startScale + (endScale - startScale) * lerpF);

  collectedPacket.whiteOverlay.setFillColor(sf::Color(255, 255, 255,
    0 + (255 - 0) * lerpF));

  if (collectedPacket.timer >= collectedPacket.duration + 0.3f) {
    onClick(*collectedPacket.sprite, []() {
      gameState = 0;
      homeState = 0;
      music.play("Menu");
      if(levelManager.currentLevel+1 == maxLevelUnlocked) maxLevelUnlocked++;
      // Add reward plant to available collection LATER
      }, []() {});
  }

}

void RewardManager::CollectedPacket::draw() {
  window->draw(whiteOverlay);
  window->draw(*sprite);
}
