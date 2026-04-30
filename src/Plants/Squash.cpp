#include <Plants/Squash.hpp>
#include <Plants/Plant.hpp>
#include <Zombies/Zombie.hpp>
#include <Grid.hpp>


void updateSquash(Plant &squash, float dt) {
  /*
  state:
  0 -> idle
  1 -> look right
  2 -> jump up
  3 -> move to top of zombie
  4 -> jump down + kill zombie
  */
  squash.reAnimator.update(dt);

  static sf::Vector2f startPos;
  static sf::Vector2f endPos;

  if (squash.state == 0) {
    int row = squash.row;

    for (int i = 0; i < zombies[row].size; i++) {
      Zombie &zombie = zombies[row][i];
      if (zombie.health <= 0) continue;

      float dx = zombie.reAnimator.getPosition().x - squash.reAnimator.getPosition().x;

      if (dx <= 110.f && dx >= 0) {
        squash.state = 1;
        squash.timer = 0.6f;
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
      squash.timer = 0.2f;
      squash.state = 2;
    }
  }
  else if (squash.state == 2) {

    if (squash.timer > 0) {
      squash.timer -= dt;
    }
    if (squash.timer <= 0) {
      squash.timer = 0.8f;
      squash.state = 3;
      startPos = squash.reAnimator.getPosition();
      endPos = grid[squash.row][squash.col].rectangle.getGlobalBounds().getCenter() + sf::Vector2f{ 100, -100 };

    }

    squash.reAnimator.playAnimation("anim_jumpup", LoopType::HoldLastFrame, 4.0f);
  }
  else if (squash.state == 3) {

    if (squash.timer > 0) {
      squash.timer -= dt;
    }
    if (squash.timer <= 0) {
      squash.timer = 0.1f;
      squash.state = 4;
      startPos = squash.reAnimator.getPosition();
      endPos = startPos + sf::Vector2f{0, 100};
      squash.reAnimator.playAnimation("anim_jumpdown", HoldLastFrame, 1.0f*2);
      squash.reAnimator.animSpeedMulti = 2.0f;
      int row = squash.row;
      for (int i = 0; i < zombies[row].size; i++) {
        Zombie& zombie = zombies[row][i];
        if (zombie.health <= 0) continue;

        float dx = zombie.reAnimator.getPosition().x - (squash.reAnimator.getPosition().x);

        if (dx <= 100.0f && dx >= -80) {
          zombie.takeDamage(SQUASH_DAMAGE, 4); // add squish damage modifier LATER
        }
      }

    }

    if(squash.timer <= 0.2f)
      squash.reAnimator.setPosition(
        startPos + (endPos - startPos) * (1 - std::min(squash.timer, 0.2f) / 0.2f)
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
        startPos + (endPos - startPos) * (1 - squash.timer / 0.1f)
      );

  }
}


void drawSquash(Plant& squash) {

}
