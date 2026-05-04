#include <LawnMower.hpp>
#include <BackgroundManager.hpp>


Array<LawnMower> LawnMower::lawnMowers;


void LawnMower::init() { // can be called multiple times to reset
  lawnMowers.erase([](LawnMower &l) {return true; });
  for (int i = 0; i < ROWS_NUMBER; i++) {
    lawnMowers.push(LawnMower{ ReAnimator(ReAnimator::getDefinition(REANIM_LAWNMOWER), 100, grid[i][0].plantPosition.y, window) });
    lawnMowers[i].reAnimator.playAnimation("anim_normal");
    lawnMowers[i].reAnimator.update(0); // init first frame and freeze it by not updating
    lawnMowers[i].reAnimator.animSpeedMulti = 2.0f;
    lawnMowers[i].isActivated = false;
    lawnMowers[i].remove = false;
  }
}

void LawnMower::updateAll(float dt) {
  for (int i = 0; i < ROWS_NUMBER; i++) {
    if (lawnMowers[i].isActivated) {
      lawnMowers[i].reAnimator.update(dt);
      lawnMowers[i].reAnimator.move({dt*350, 0});
      if (lawnMowers[i].reAnimator.getPosition().x >= 1130) {
        lawnMowers[i].remove = true;
        lawnMowers[i].isActivated = false;
      }

      for (int z = 0; z < zombies[i].size; z++) {
        if (zombies[i][z]->reAnimator.getPosition().x <= lawnMowers[i].reAnimator.getPosition().x && zombies[i][z]->health > 0) {
          zombies[i][z]->takeDamage(5000.0f, 3);
          zombies[i][z]->reAnimator.animSpeedMulti = 0.0f; // freeze zombie animation

        }
      }

    }
  }
}


void LawnMower::drawAll() {
  window->setView(*gameView);
  for (int i = 0; i < ROWS_NUMBER; i++) {
    //std::cout << "X: " << lawnMowers[i].reAnimator.trackInstances.size() << "\n";
    //lawnMowers[i].reAnimator.update(0.01);
    //lawnMowers[i].reAnimator.drawHitbox();

    /*sf::CircleShape cerc(2);
    cerc.setPosition(lawnMowers[i].reAnimator.getPosition());
    window->draw(cerc);*/

    if(!lawnMowers[i].remove)
      lawnMowers[i].reAnimator.draw();
  }
  window->setView(dayLevel.camera);
}


void LawnMower::activateLawnMower(int row) {
  if (!lawnMowers[row].remove && !lawnMowers[row].isActivated) {
    lawnMowers[row].isActivated = true;
    sounds.play("Lawnmower");
  }
}
