#include <Plants/SunFlower.hpp>
#include <SunManager.hpp>
#include <Plants/Plant.hpp>

void updateSunFlower(Plant &sunFlower, float dt){
  sunFlower.reAnimator.update(dt);

  sunFlower.timer -= dt;
  sunFlower.blinkTimer -= dt;
  if (sunFlower.timer <= 0){
    Sounds::play("sunFlowerPop");
    Sun::generate({sunFlower.reAnimator.getPosition()}, 25, 1);
    sunFlower.timer = GENERATE_SUN_FLOWER_INTERVAL;
  }
  
  if (sunFlower.blinkTimer <= 0) {
    sunFlower.reAnimator.playAnimation("blink", LoopType::PlayOnce);
    sunFlower.blinkTimer = randomRange(1.5f, 3.0f);
  }
}

void drawSunFlower(Plant &sunFlower){
  if (sunFlower.timer <= 2.5f) {// glow up
    sunFlower.reAnimator.setOverlayAlpha(1 - sunFlower.timer / 2.5f);
  }
else if (sunFlower.timer >= 9.0f) { // glow down
    sunFlower.reAnimator.setOverlayAlpha(1 - (10.0f - sunFlower.timer) / 1.0f);
  }
}
