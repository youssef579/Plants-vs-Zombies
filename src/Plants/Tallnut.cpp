#include <Plants/SunFlower.hpp>
#include <SunManager.hpp>
#include <Plants/Plant.hpp>
#include <Plants/Tallnut.hpp>

void updateTallnut(Plant &tallnut, float dt){
  //animateSpritesheet(tallNut.sheet, dt);
  tallnut.reAnimator.update(dt);
  tallnut.health -= dt * 10;
  tallnut.blinkTimer -= dt;

  if (tallnut.blinkTimer <= 0) {
    int numberOfBlinks = randomRange(2, 3);

    switch (numberOfBlinks) {
    case 2:
      tallnut.reAnimator.playAnimation("anim_blink_twice", false);
      break;
    case 3:
      tallnut.reAnimator.playAnimation("anim_blink_thrice", false);
      break;

    }
    tallnut.blinkTimer = randomRange(1.5f, 4.0f);
  }

  if (tallnut.health <= TALLNUT_CRACK1_HEALTH && !tallnut.timer){
    tallnut.reAnimator.trackInstances[0].imageOverride
      = tallnut.reAnimator.reAnimDef->textureMap["IMAGE_REANIM_TALLNUT_CRACKED1"];

    tallnut.timer = 1;
    
  }else if (tallnut.health <= TALLNUT_CRACK2_HEALTH && tallnut.timer == 1){
    tallnut.reAnimator.trackInstances[0].imageOverride
      = tallnut.reAnimator.reAnimDef->textureMap["IMAGE_REANIM_TALLNUT_CRACKED2"];

    tallnut.timer = 2;
  }
}

void drawTallnut(Plant &tallnut){

}
