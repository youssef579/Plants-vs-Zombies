#include <Plants/Squash.hpp>
#include <Plants/Plant.hpp>
#include <Zombies/Zombie.hpp>
#include <Grid.hpp>



struct SquashData {
  sf::Vector2f startPos;
  sf::Vector2f endPos;
  Zombie *closestZombie = nullptr; // might cause problem if zombie deletes before squash (e7tmal so8yr gdan)
  sf::Vector2f lastZombiePos;
};

// static to make it not global to project
static SquashData squashData[ROWS_NUMBER][COLUMNS_NUMBER]; // store extra data unique to squash

void updateSquash(Plant &squash, float dt) {
  /*
  state:
  0 -> idle (search for zombies)
  1 -> look right (store closest zombie & last position)
  2 -> jump up
  3 -> move to top of zombie (try moving to zombie pos else move to last position)
  4 -> jump down + kill zombie (kill aoe around squash)
  */
  squash.reAnimator.update(dt);

  // Separate start and end positions to each unique squash LATER
  //static sf::Vector2f startPos;
  //static sf::Vector2f endPos;

  SquashData &data = squashData[squash.row][squash.col];

  if (squash.state == 0) {
    int row = squash.row;

    for (int i = 0; i < zombies[row].size; i++) {
      Zombie *zombie = zombies[row][i];
      if (zombie->health <= 0) continue;

      float dx = zombie->reAnimator.getPosition().x - squash.reAnimator.getPosition().x;

      if (dx <= 125.f && dx >= 0) {
        squash.state = 1;
        squash.timer = 0.6f;
        data.closestZombie = zombie;
        data.lastZombiePos = { zombie->reAnimator.getPosition().x, squash.reAnimator.getPosition().y };
        break;
      }
    }
  }
  else if (squash.state == 1) {

    if (!squash.reAnimator.isPlayingAnimation("anim_lookright")) {
      squash.reAnimator.stopAnimation("anim_idle");
      squash.reAnimator.playAnimation("anim_lookright", LoopType::HoldLastFrame, 0.6f);
    }

    if (squash.timer > 0) {
      squash.timer -= dt;
    }
    if (squash.timer <= 0) {
      squash.timer = 0.1f;
      squash.state = 2;
    }
  }
  else if (squash.state == 2) {

    if (squash.timer > 0) {
      squash.timer -= dt;
    }
    if (squash.timer <= 0) {
      squash.timer = 0.6f;
      squash.state = 3;
      data.startPos = squash.reAnimator.getPosition();
      //data.endPos = grid[squash.row][squash.col].rectangle.getGlobalBounds().getCenter() + sf::Vector2f{ 100, -100 };
      if (data.closestZombie) {
        data.endPos = { data.closestZombie->reAnimator.getPosition().x, squash.reAnimator.getPosition().y - 100.0f };
      }else data.endPos = data.lastZombiePos - sf::Vector2f{ 0, 100.0f };
    }

    squash.reAnimator.playAnimation("anim_jumpup", LoopType::HoldLastFrame, 4.0f);
  }
  else if (squash.state == 3) {

    if (squash.timer > 0) {
      squash.timer -= dt;
    }
    if (squash.timer <= 0) {
      squash.timer = 0.075f;
      squash.state = 4;
      data.startPos = squash.reAnimator.getPosition();
      if (data.closestZombie)
        data.endPos = { data.closestZombie->reAnimator.getPosition().x, squash.reAnimator.getPosition().y + 100.0f };
      else data.endPos = data.lastZombiePos + sf::Vector2f{ 0, 100.0f };


      squash.reAnimator.playAnimation("anim_jumpdown", HoldLastFrame, 1.5f);
      squash.reAnimator.animSpeedMulti = 1.7f;
      int row = squash.row;
      for (int i = 0; i < zombies[row].size; i++) {
        Zombie *zombie = zombies[row][i];
        if (zombie->health <= 0) continue;

        float dx = zombie->reAnimator.getPosition().x - (squash.reAnimator.getPosition().x);

        if (dx <= 50.0f && dx >= -50) { // adjust aoe LATER
          zombie->takeDamage(SQUASH_DAMAGE, 4); // add squish damage modifier LATER
        }
      }

    }

    if(squash.timer <= 0.15f)
      squash.reAnimator.setPosition(
        data.startPos + (data.endPos - data.startPos) * (1 - std::min(squash.timer, 0.15f) / 0.15f)
      );
  }
  else if (squash.state == 4) {
    if (squash.timer > 0) {
      squash.timer -= dt;
    }

    if (!squash.reAnimator.isPlayingAnimation("anim_jumpdown"))
      squash.health = 0;

    //if (squash.timer <= 0.3f)
      squash.reAnimator.setPosition(
        data.startPos + (data.endPos - data.startPos) * (1 - squash.timer / 0.075f)
      );

  }
}


void drawSquash(Plant& squash) {

}
