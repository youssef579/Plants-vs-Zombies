#include <Plants/SunFlower.hpp>
#include <SunManager.hpp>
#include <Plants/Plant.hpp>
#include <Plants/Wallnut.hpp>


/*Plant createWallnut(float x, float y /* the way used in grid , int row) {
  static sf::Texture& wallNutTexture = getTexture("assets/Plants/wallnut.png");
  wallNutTexture.setSmooth(true);

  sf::Sprite wallNutSprite(wallNutTexture);
  wallNutSprite.setTextureRect({{0, 0}, {65, 73}});

  Plant newPlant = {wallNutSprite, WALLNUT_HEALTH, PlantType::WALLNUT, 0, row, 0,nullptr};
  Spritesheet wallNutSheet = Spritesheet{&newPlant.sprite, 65, 73, 16, 0.04f};
  newPlant.sheet = wallNutSheet;

  newPlant.sprite.setOrigin(newPlant.sprite.getLocalBounds().size / 2.0f);

  newPlant.sprite.setPosition({x, y});

  return newPlant;
}*/

void updateWallnut(Plant &wallNut, float dt){
  animateSpritesheet(wallNut.sheet, dt);
  wallNut.health -= dt * 5;
  if (wallNut.health <= CRACK1_HEALTH && !wallNut.timer){
    static sf::Texture& wallNutCrackTexture = getTexture("assets/Plants/wallnutCrack.png");
    wallNutCrackTexture.setSmooth(true);

    wallNut.sprite.setTexture(wallNutCrackTexture);
    wallNut.sprite.setTextureRect({{0, 0}, {65, 73}});
    wallNut.timer = 1;
    
    Spritesheet wallNutSheet = Spritesheet{&wallNut.sprite, 65, 73, 11, 0.0581f};
    wallNut.sheet = wallNutSheet;
  }else if (wallNut.health <= CRACK2_HEALTH && wallNut.timer == 1){
    static sf::Texture& wallNutCrack2Texture = getTexture("assets/Plants/wallnutCrack2.png");
    wallNutCrack2Texture.setSmooth(true);

    wallNut.sprite.setTexture(wallNutCrack2Texture);
    wallNut.sprite.setTextureRect({{0, 0}, {65, 73}});
    wallNut.timer = 2;
    
    Spritesheet wallNutSheet = Spritesheet{&wallNut.sprite, 65, 73, 15, 0.0426f};
    wallNut.sheet = wallNutSheet;
  }
}

void drawWallnut(Plant &wallNut){
    window->draw(wallNut.sprite);
}
