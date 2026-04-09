#include <Plants/SunFlower.hpp>
#include <SunManager.hpp>
#include <Plants/Plant.hpp>
#include <iostream>

Plant createWallnut(float x, float y /* the way used in grid */) {
  static sf::Texture& wallNutTexture = getTexture("assets/wallnut.png");
  sf::Sprite wallNutSprite(wallNutTexture);
  wallNutSprite.setTextureRect({{0, 0}, {65, 73}});

  Plant newPlant = {wallNutSprite, 100, PlantType::WALLNUT, 0, nullptr};
  Spritesheet wallNutSheet = Spritesheet{&newPlant.sprite, 65, 73, 16, 0.04f};
  newPlant.sheet = wallNutSheet;
  
  newPlant.sprite.setOrigin(newPlant.sprite.getLocalBounds().size / 2.0f);

  newPlant.sprite.setPosition({x, y});

  return newPlant;
}

void updateWallnut(Plant &wallNut, float dt){
  animateSpritesheet(wallNut.sheet, dt);
  static bool onceCrack = 0, onceCrack2 = 0;
  if (wallNut.health <= 50 && !onceCrack){
    static sf::Texture& wallNutCrackTexture = getTexture("assets/wallnutCrack.png");
    sf::Sprite wallNutCrackSprite(wallNutCrackTexture);
    wallNutCrackSprite.setTextureRect({{0, 0}, {65, 73}});

    sf::Vector2f wallNutPostion = wallNut.sprite.getPosition();

    wallNut = {wallNutCrackSprite, 50, PlantType::WALLNUT, 0, nullptr};
    Spritesheet wallNutSheet = Spritesheet{&wallNut.sprite, 65, 73, 11, 0.0581f};
    wallNut.sheet = wallNutSheet;
  
    wallNut.sprite.setOrigin(wallNut.sprite.getLocalBounds().size / 2.0f);

    wallNut.sprite.setPosition(wallNutPostion);

    onceCrack = 1;
  }else if (wallNut.health <= 20 && !onceCrack2){
    static sf::Texture& wallNutCrack2Texture = getTexture("assets/wallnutCrack2.png");
    sf::Sprite wallNutCrack2Sprite(wallNutCrack2Texture);
    wallNutCrack2Sprite.setTextureRect({{0, 0}, {65, 73}});

    sf::Vector2f wallNutPostion = wallNut.sprite.getPosition();

    wallNut = {wallNutCrack2Sprite, 20, PlantType::WALLNUT, 0, nullptr};
    Spritesheet wallNutSheet = Spritesheet{&wallNut.sprite, 65, 73, 15, 0.0426f};
    wallNut.sheet = wallNutSheet;
  
    wallNut.sprite.setOrigin(wallNut.sprite.getLocalBounds().size / 2.0f);

    wallNut.sprite.setPosition(wallNutPostion);

    onceCrack2 = 1;
  }
}

void drawWallnut(Plant &wallNut){
    window->draw(wallNut.sprite);
}