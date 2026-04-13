#include <Plants/SunFlower.hpp>
#include <SunManager.hpp>
#include <Plants/Plant.hpp>
#include <Plants/Wallnut.hpp>


Plant createWallnut(float x, float y /* the way used in grid */, int row) {
  static sf::Texture& wallNutTexture = getTexture("assets/Plants/wallnut.png");
  sf::Sprite wallNutSprite(wallNutTexture);
  wallNutSprite.setTextureRect({{0, 0}, {65, 73}});

  Plant newPlant = {wallNutSprite, WALLNUT_HEALTH, PlantType::WALLNUT, 0, row, nullptr};
  Spritesheet wallNutSheet = Spritesheet{&newPlant.sprite, 65, 73, 16, 0.04f};
  newPlant.sheet = wallNutSheet;

  newPlant.sprite.setOrigin(newPlant.sprite.getLocalBounds().size / 2.0f);

  newPlant.sprite.setPosition({x, y});

  return newPlant;
}

void updateWallnut(Plant &wallNut, float dt){
  animateSpritesheet(wallNut.sheet, dt);
  if (wallNut.health <= CRACK1_HEALTH && !wallNut.timer){
    static sf::Texture& wallNutCrackTexture = getTexture("assets/Plants/wallnutCrack.png");
    sf::Sprite wallNutCrackSprite(wallNutCrackTexture);
    wallNutCrackSprite.setTextureRect({{0, 0}, {65, 73}});

    sf::Vector2f wallNutPostion = wallNut.sprite.getPosition();
    int row = wallNut.row;

    wallNut = {wallNutCrackSprite, CRACK1_HEALTH, PlantType::WALLNUT, 1, row, nullptr};
    Spritesheet wallNutSheet = Spritesheet{&wallNut.sprite, 65, 73, 11, 0.0581f};
    wallNut.sheet = wallNutSheet;

    wallNut.sprite.setOrigin(wallNut.sprite.getLocalBounds().size / 2.0f);

    wallNut.sprite.setPosition(wallNutPostion);

  }else if (wallNut.health <= CRACK2_HEALTH && wallNut.timer == 1){
    static sf::Texture& wallNutCrack2Texture = getTexture("assets/Plants/wallnutCrack2.png");
    sf::Sprite wallNutCrack2Sprite(wallNutCrack2Texture);
    wallNutCrack2Sprite.setTextureRect({{0, 0}, {65, 73}});

    sf::Vector2f wallNutPostion = wallNut.sprite.getPosition();
    int row = wallNut.row;

    wallNut = {wallNutCrack2Sprite, CRACK2_HEALTH, PlantType::WALLNUT, 2, row, nullptr};
    Spritesheet wallNutSheet = Spritesheet{&wallNut.sprite, 65, 73, 15, 0.0426f};
    wallNut.sheet = wallNutSheet;

    wallNut.sprite.setOrigin(wallNut.sprite.getLocalBounds().size / 2.0f);

    wallNut.sprite.setPosition(wallNutPostion);

  }
}

void drawWallnut(Plant &wallNut){
    window->draw(wallNut.sprite);
}
