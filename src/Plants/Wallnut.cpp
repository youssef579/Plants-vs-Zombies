#include <Plants/SunFlower.hpp>
#include <SunManager.hpp>
#include <Plants/Plant.hpp>
#include <Plants/Wallnut.hpp>

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
