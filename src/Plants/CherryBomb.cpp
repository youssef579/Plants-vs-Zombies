#include <Plants/CherryBomb.hpp>
#include <Plants/Plant.hpp>
#include <Zombies/Zombie.hpp>
#include <Grid.hpp>
#include <ParticleSystem.hpp>



void updateCherryBomb(Plant& cherryBomb, float dt) {

  cherryBomb.reAnimator.update(dt);

  if (cherryBomb.timer > 0) {
    cherryBomb.timer -= dt;
  }

  if (cherryBomb.timer <= 0 && cherryBomb.health > 0) {
    sounds.play("CherryBomb");
    ParticleSystem::spawnParticleEffect(PEFF_POWIE, cherryBomb.reAnimator.getPosition());
    for (int row = cherryBomb.row - 1; row <= cherryBomb.row + 1; row++) {

      if (row < 0 || row >= ROWS_NUMBER)continue;

      for (int i = 0; i < zombies[row].size;i++) {
        Zombie *zombie = zombies[row][i];

        if (zombie->health <= 0)continue;

        float dx = std::abs(zombie->reAnimator.getPosition().x - cherryBomb.reAnimator.getPosition().x);

        if (dx <= 160.f) {
          zombie->takeDamage(5000.f, 2);
        }
      }
    }
    cherryBomb.health = 0;
  }
}

void drawCherryBomb(Plant& cherryBomb) {

}
