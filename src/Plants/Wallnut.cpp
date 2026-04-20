#include <Plants/SunFlower.hpp>
#include <SunManager.hpp>
#include <Plants/Plant.hpp>
#include <Plants/Wallnut.hpp>

void updateWallnut(Plant &wallNut, float dt){
  //animateSpritesheet(wallNut.sheet, dt);
  wallNut.reAnimator.update(dt);
  //wallNut.health -= dt * 5;
  wallNut.blinkTimer -= dt;

  if (wallNut.blinkTimer <= 0) {
    int numberOfBlinks = randomRange(2, 3);
    //std::cout << "Playing " << numberOfBlinks << " blinks\n";
    switch (numberOfBlinks) {
    case 2:
      wallNut.reAnimator.playAnimation("anim_blink_twice", LoopType::PlayOnce);
      break;
    case 3:
      wallNut.reAnimator.playAnimation("anim_blink_thrice", LoopType::PlayOnce);
      break;

    }
    wallNut.blinkTimer = randomRange(1.5f, 4.0f);
  }

  if (wallNut.health <= WALLNUT_CRACK1_HEALTH && !wallNut.timer){
    wallNut.reAnimator.trackInstances[2].imageOverride
      = wallNut.reAnimator.reAnimDef->textureMap["IMAGE_REANIM_WALLNUT_CRACKED1"];
    wallNut.reAnimator.playAnimation("anim_blink_twitch", LoopType::PlayOnce);

    wallNut.timer = 1;
    
  }else if (wallNut.health <= WALLNUT_CRACK2_HEALTH && wallNut.timer == 1){
    wallNut.reAnimator.trackInstances[2].imageOverride
      = wallNut.reAnimator.reAnimDef->textureMap["IMAGE_REANIM_WALLNUT_CRACKED2"];
    wallNut.reAnimator.playAnimation("anim_blink_twitch", LoopType::PlayOnce);

    wallNut.timer = 2;
    
  }
}

void drawWallnut(Plant &wallNut){

}
