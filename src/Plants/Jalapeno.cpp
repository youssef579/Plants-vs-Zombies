#include <Plants/Jalapeno.hpp>
#include <Plants/Plant.hpp>
#include <Zombies/Zombie.hpp>
#include <Grid.hpp>
#include <ParticleSystem.hpp>



void updateJalapeno(Plant& jalapeno, float dt) {

  jalapeno.reAnimator.update(dt);

  if (jalapeno.timer > 0) {
    jalapeno.timer -= dt;
  }

  if (jalapeno.timer <= 0 && jalapeno.health > 0) {
    //ParticleSystem::spawnParticleEffect(PEFF_POWIE, jalapeno.reAnimator.getPosition());

    if (jalapeno.state == 0)
      for (int i = 0; i < zombies[jalapeno.row].size; i++)
        if (zombies[jalapeno.row][i].health > 0 && zombies[jalapeno.row][i].inPlayArea)
          zombies[jalapeno.row][i].takeDamage(5000.0f, 2);

    if (jalapeno.state == 0) {
      sounds.play("Jalapeno");
      //fireArray.erase([](ReAnimator &fire) { return true; }); // clear
      /*for (int i = 0; i < COLUMNS_NUMBER; i++) {
        ReAnimator::orphanAnimators.push(ReAnimator(ReAnimator::getDefinition(REANIM_FIRE), grid[jalapeno.row][i].plantPosition.x, grid[jalapeno.row][i].plantPosition.y, window));
        ReAnimator::orphanAnimators[ReAnimator::orphanAnimators.size - 1].playAnimation("anim_flame", LoopType::LoopTimes, (i + 1)*5);
      }*/
      for (int x = grid[jalapeno.row][0].plantPosition.x - 50, i=0;
        x <= grid[jalapeno.row][COLUMNS_NUMBER - 1].plantPosition.x + 80; x += 60, i++) {
          ReAnimator::orphanAnimators.push(ReAnimator(ReAnimator::getDefinition(REANIM_FIRE), x, grid[jalapeno.row][jalapeno.col].plantPosition.y, window));
          ReAnimator::orphanAnimators[ReAnimator::orphanAnimators.size - 1].playAnimation("anim_flame", LoopType::LoopTimes, (i + 1) * 4 + 5);
      }
      jalapeno.state = 1;
    }
    

    jalapeno.health = 0;
    /*if(!fireArray[COLUMNS_NUMBER-1].isPlayingAnimation("anim_flame"))
      jalapeno.health = 0;
    else
      for (int i = 0; i < COLUMNS_NUMBER; i++) {
        fireArray[i].update(dt);
        fireArray[i].draw();
      }*/
  }
}

void drawJalapeno(Plant& jalapeno) {

}
