#include <Plants/iceShroom.hpp>
#include <Plants/Plant.hpp>
#include <Zombies/Zombie.hpp>
#include <Grid.hpp>
#include <ParticleSystem.hpp>



void updateIceShroom(Plant& iceShroom, float dt) {

  iceShroom.reAnimator.update(dt);

  if (iceShroom.timer > 0) {
    iceShroom.timer -= dt;
  }

  if (iceShroom.timer <= 0 && iceShroom.health > 0) {
    // صوت انفجار متلج
    // انيميشن انفجار متلج
    //ParticleSystem::spawnParticleEffect(PEFF_POWIE, iceShroom.reAnimator.getPosition());
    for (int row = 0; row < ROWS_NUMBER; row++) {

      for (int i = 0; i < zombies[row].size;i++) {
        Zombie& zombie = zombies[row][i];

        if (zombie.health <= 0)continue;

        if (zombie.inPlayArea){
          zombie.freezeTimer = 5.0f;
          zombie.takeDamage(20, 0);
        }
      }

    }
    iceShroom.health = 0;
  }
}

void drawIceShroom(Plant &iceShroom) {

}
