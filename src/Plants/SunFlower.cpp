#include <Plants/SunFlower.hpp>
#include <SunManager.hpp>
#include <Plants/Plant.hpp>

void updateSunFlower(Plant &sunFlower, float dt){
  animateSpritesheet(sunFlower.sheet, dt);
  sunFlower.timer -= dt;
  if (sunFlower.timer <= 0){
    Sounds::play("sunFlowerPop");
    Sun::generate(sunFlower.sprite.getPosition().x, sunFlower.sprite.getPosition().y, 50, 1);
    sunFlower.timer = GENERATE_SUN_FLOWER_INTERVAL;
  }
}

void drawSunFlower(Plant &sunFlower){
    window->draw(sunFlower.sprite);
    if (sunFlower.timer <= SHINE_TIME)
      window->draw(sunFlower.sprite, sf::RenderStates(sf::BlendAdd));
}
