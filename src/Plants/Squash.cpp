#include <Plants/Squash.hpp>
#include <Plants/Plant.hpp>
#include <Zombies/Zombie.hpp>
#include <Grid.hpp>

void updateSquash(Plant& squash, float dt) {
  squash.reAnimator.update(dt);
  if (squash.state == 0) {
    int row = squash.row;

    for (int i = 0; i < zombies[row].size; i++) {
      Zombie& zombie = zombies[row][i];
      if (zombie.health <= 0)continue;

      float dx = zombie.reAnimator.getPosition().x - squash.reAnimator.getPosition().x;

      if (dx <= 110.f && dx>=0) {
        squash.state = 1;
        squash.timer = SQUASH_DELAY_TIME;
        break;
      }
    }
  }
  if (squash.state == 1) {
    if (squash.timer > 0) {
      squash.timer -= dt;
    }
    if (squash.timer <= 0) {
      int row = squash.row;

      for (int i = 0; i < zombies[row].size; i++) {
        Zombie& zombie = zombies[row][i];
        if (zombie.health <= 0)continue;

        float dx = zombie.reAnimator.getPosition().x - squash.reAnimator.getPosition().x;

        if (dx <= 110.f && dx >= 0) {
          zombie.takeDamage(SQUASH_DAMAGE);
        }
      }
      squash.health = 0;
    }
  }
}

void drawSquash(Plant& squash) {

}
