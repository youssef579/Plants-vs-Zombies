#include <PlantsSelector .hpp>
#include <Window.hpp>
#include <AssetsManager.hpp>

PlantsSelector plantSelector;

void initSelector() {
  plantSelector.currX = -700.f;
  plantSelector.tarX  = -700.f;
  plantSelector.speed = 1000.f;
  plantSelector.isVisible = false;
  plantSelector.isSlidingOut = false;

  if (!plantSelector.backSprite) {

    plantSelector.backSprite = new sf::Sprite(getTexture("assets/plantsSelector/selectorBackground.png"));
    plantSelector.backSprite->setOrigin(plantSelector.backSprite->getLocalBounds().size / 2.0f);

  }

  plantSelector.playBtnNormal = getTexture("assets/plantsSelector/SeedChooser_Button_Disabled.png");
  plantSelector.playBtnHover = getTexture("assets/plantsSelector/SeedChooser_Button.png");

  if (!plantSelector.playBtn) {
    plantSelector.playBtn = new sf::Sprite(getTexture("assets/plantsSelector/SeedChooser_Button_Disabled.png"));
    plantSelector.playBtn->setOrigin(plantSelector.playBtn->getLocalBounds().size / 2.0f);
  }
}

void slideIn() {
  plantSelector.tarX = 230.0f;
  plantSelector.isVisible = true;
  plantSelector.isSlidingOut = false;
}

void slideOut() {
  plantSelector.tarX = -600.0f;
  plantSelector.isSlidingOut = true;
}

void updateSelector(float dt , sf::RenderWindow& window){
  if (plantSelector.currX < plantSelector.tarX) {
    plantSelector.currX += plantSelector.speed * dt;
    if (plantSelector.currX > plantSelector.tarX) plantSelector.currX = plantSelector.tarX; 
  }
  else if (plantSelector.currX > plantSelector.tarX) {
    plantSelector.currX -= plantSelector.speed * dt;
    if(plantSelector.currX < plantSelector.tarX) plantSelector.currX = plantSelector.tarX;
  }

  if (plantSelector.backSprite) plantSelector.backSprite->setPosition({ plantSelector.currX , 350.f });
  if (plantSelector.playBtn) {
    plantSelector.playBtn->setPosition({ plantSelector.currX , 570.f });

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos, window.getDefaultView());

    if (plantSelector.playBtn->getGlobalBounds().contains(worldPos)) {
      plantSelector.playBtn->setTexture(plantSelector.playBtnHover);
    }
    else {
      plantSelector.playBtn->setTexture(plantSelector.playBtnNormal);
    }
  }
  if (plantSelector.isSlidingOut && plantSelector.currX <= -680.f) {
    plantSelector.isVisible = false;
  }
}

  void drawSelector(sf::RenderWindow& window) {

    if (plantSelector.currX  > -590.f) {
      if (plantSelector.backSprite) window.draw(*plantSelector.backSprite);
      if (plantSelector.playBtn) window.draw(*plantSelector.playBtn);
    }
  }


