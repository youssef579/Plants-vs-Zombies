#include <Plants/potatoMine.hpp>
#include <Plants/Plant.hpp>
#include <Zombies/Zombie.hpp>
#include <Grid.hpp>
#include <ParticleSystem.hpp>

void updatePotatoMine(Plant &potatoMine, float dt) {

  //the animation here
  potatoMine.reAnimator.update(dt);
  if (potatoMine.timer > 0){
    potatoMine.timer -= dt;
  }

  if (potatoMine.timer <= 0 && !potatoMine.state){
    // صوت بطاطس بتطلع مش عارف موجود في اللعبة ولا لا
    //انيميشن بطاطس بتطلع + تثبت 

    
    potatoMine.state = 1;
  }else if (potatoMine.timer <= 0 && potatoMine.state == 1){

    bool is_explosion = 0;
    for (int i = 0; i < zombies[potatoMine.row].size;i++) {
      Zombie& zombie = zombies[potatoMine.row][i];
      
      if (zombie.health <= 0) continue;

      float dx = std::abs(zombie.reAnimator.getPosition().x - potatoMine.reAnimator.getPosition().x);
      if (dx <= 65.f && dx >= -70.0f) {
        is_explosion = 1;
        zombie.takeDamage(5000.f, 2);
      }
    }

    if (is_explosion){
      // صوت انفجار
      // انيميشن الانفجار
      ParticleSystem::spawnParticleEffect(PEFF_POWIE, potatoMine.reAnimator.getPosition());
      potatoMine.health = 0;
    }
  }
}

void drawPotatoMine(Plant &potatoMine) {

}
