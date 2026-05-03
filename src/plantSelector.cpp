#include <PlantSelector.hpp>
#include <Window.hpp>
#include <AssetsManager.hpp>
#include <BackgroundManager.hpp>
#include <iostream>
#include <Packets/Packet.hpp>

Array<PlantType> chosenPlants;

PlantsSelector plantSelector;
extern BackgroundManager dayLevel;
static float globOffsetX = -300.0f;
extern sf::View* view;

sf::Vector2f getSelectedSlotPos(int index) {
  float startX = 85.0f;
  float startY = 45.0f;
  float gapX = 52.0f;
  return { startX + (index * gapX) , startY };
}

sf::Vector2f getPacketPosition(int index) {
  float startOffsetX = 25.0f;
  float startOffsetY = -200.0f;
  float gapX = 90.f;
  float gapY = 100.0f;
  int columns = 5;
  int row = index / columns;
  int col = index % columns;
  float x = plantSelector.currX + startOffsetX + (col * gapX);
  float y = 350.0f + startOffsetY + (row * gapY);

  return { x , y };

}

void PlantsSelector::initSelector() {
  plantSelector.currX = -600.f;
  plantSelector.tarX  = -600.f;
  plantSelector.speed = 1000.f;
  plantSelector.isVisible = false;
  plantSelector.isSlidingOut = false;

  if (!plantSelector.backSprite) {

    plantSelector.backSprite = new sf::Sprite(getTexture("assets/plantsSelector/selectorBackground.png"));
    //plantSelector.backSprite->setOrigin(plantSelector.backSprite->getLocalBounds().size / 2.0f);
    plantSelector.backSprite->setOrigin({ 0.f, plantSelector.backSprite->getLocalBounds().size.y / 2.0f });
    //float width = plantSelector.backSprite->getLocalBounds().size.x;
    //plantSelector.backSprite->setOrigin({ width, plantSelector.backSprite->getLocalBounds().size.y / 2.0f });

  }

  plantSelector.playBtnNormal = getTexture("assets/plantsSelector/SeedChooser_Button_Disabled.png");
  plantSelector.playBtnHover = getTexture("assets/plantsSelector/SeedChooser_Button.png");

  if (!plantSelector.playBtn) {
    plantSelector.playBtn = new sf::Sprite(getTexture("assets/plantsSelector/SeedChooser_Button_Disabled.png"));
    plantSelector.playBtn->setOrigin(plantSelector.playBtn->getLocalBounds().size / 2.0f);
    //plantSelector.playBtn->setOrigin({ 0.f, plantSelector.playBtn->getLocalBounds().size.y / 2.0f });
  }

  for (int i = 0; i < packetsNum; i++) {
    plantSelector.packets[i].id = i;
    sf::Texture& tex = getTexture("assets/packets/" + std::to_string(i) + ".png");
   // std::cout << i << '\n';
    plantSelector.packets[i].sprite = new sf::Sprite(tex);
    plantSelector.packets[i].sprite->setScale({ 1.2f , 1.2f });       
  }
}

void PlantsSelector::slideIn() {
  plantSelector.tarX = 1.0f;
  plantSelector.isVisible = true;
  plantSelector.isSlidingOut = false;
}

void PlantsSelector::slideOut() {
  plantSelector.tarX = -600.0f;
  plantSelector.isSlidingOut = true;
}

void PlantsSelector::updateSelector(float dt , sf::RenderWindow& window){
  
  sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window), *view);
  if (plantSelector.currX < plantSelector.tarX) {
    plantSelector.currX += plantSelector.speed * dt;
    if (plantSelector.currX > plantSelector.tarX) plantSelector.currX = plantSelector.tarX;
  }
  else if (plantSelector.currX > plantSelector.tarX) {
    plantSelector.currX -= plantSelector.speed * dt;
    if (plantSelector.currX < plantSelector.tarX) plantSelector.currX = plantSelector.tarX;
  }

  if (plantSelector.backSprite)
    plantSelector.backSprite->setPosition({ plantSelector.currX , 350.f });

  

  if (plantSelector.playBtn) {
    plantSelector.playBtn->setPosition({ plantSelector.currX + 230.f , 570.f });
    if (plantSelector.playBtn->getGlobalBounds().contains(worldPos)) {
      plantSelector.playBtn->setTexture(plantSelector.playBtnHover);
      if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (chosenPlants.size > 0) {
          fillPackets(chosenPlants);
          plantSelector.currentSelectedCnt = 0;
          for (int j = 0; j < packetsNum; j++) {
            plantSelector.packets[j].isSelected = false;
            plantSelector.packets[j].sprite->setColor(sf::Color::White);
          }
          for (int s = 0; s < 7; s++) {
            plantSelector.selectedSlot[s].active = false;
            plantSelector.selectedSlot[s].sprite = nullptr;
          }
        }
        slideOut();
        dayLevel.playBtnClicked = true;
        dayLevel.isWaitingForPlay = false;
        dayLevel.introTimer = 5.5f;
       // chosenPlants.erase([](PlantType& p) { return true; });
        chosenPlants.size = 0;
      }
    }
    else {
      plantSelector.playBtn->setTexture(plantSelector.playBtnNormal);
    }
  }
  
  static bool mousePressedLastFrame = false;
  bool isLeftPressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

  for (int i = 0; i < packetsNum ; i++) {
   
    plantSelector.packets[i].sprite->setPosition(getPacketPosition(i));
    
    if (plantSelector.isVisible && !plantSelector.packets[i].isSelected &&
      (plantSelector.currentSelectedCnt < 7)) {
      if (plantSelector.packets[i].sprite->getGlobalBounds().contains(worldPos)) {
        if (isLeftPressed && !mousePressedLastFrame){
          int emptySlot = -1;
          for(int s=0 ; s<7 ; s++){
            if (!plantSelector.selectedSlot[s].active) {
              emptySlot = s;
              break;
            }
          }
          if (emptySlot != -1) {
            chosenPlants.push((PlantType)plantSelector.packets[i].id);

            plantSelector.selectedSlot[emptySlot].id = plantSelector.packets[i].id;
            plantSelector.selectedSlot[emptySlot].active = true;
            plantSelector.selectedSlot[emptySlot].sprite = new sf::Sprite(plantSelector.packets[i].sprite->getTexture());

            float startX = 90.0f;
            float startY = 15.0f;
            float gapX = 60.0f;
            plantSelector.selectedSlot[emptySlot].sprite->setPosition({ startX + (emptySlot * gapX), startY });

            plantSelector.packets[i].isSelected = true;
            plantSelector.packets[i].sprite->setColor(sf::Color(100, 100, 100));
            plantSelector.currentSelectedCnt++;
          }
          //int slot = plantSelector.currentSelectedCnt;
          //chosenPlants.push((PlantType)plantSelector.packets[i].id);
          //plantSelector.selectedSlot[slot].id = plantSelector.packets[i].id;
          //plantSelector.selectedSlot[slot].active = true;
          //plantSelector.selectedSlot[slot].sprite = new sf::Sprite(plantSelector.packets[i].sprite->getTexture());
          ////plantSelector.selectedSlot[slot].sprite->setScale({ 0.75f, 0.75f });
          //float startX = 90.0f;
          //float startY = 15.0f; 
          //float gapX = 60.0f;
          //selectedSlot[slot].sprite->setPosition({ startX + (slot * gapX), startY });
          //plantSelector.packets[i].isSelected = true;
          //plantSelector.packets[i].sprite->setColor(sf::Color(100, 100, 100));
          //plantSelector.currentSelectedCnt++;

        }
      }
    }
  }
  if (plantSelector, isVisible) {
    for (int i = 0; i < 7; i++) {
      if (plantSelector.selectedSlot[i].active && plantSelector.selectedSlot[i].sprite) {
        if (plantSelector.selectedSlot[i].sprite->getGlobalBounds().contains(worldPos)) {
          if (isLeftPressed && !mousePressedLastFrame) {

            int removedId = plantSelector.selectedSlot[i].id;

            for (int j = 0; j < packetsNum; j++) {
              if (plantSelector.packets[j].id == removedId) {
                plantSelector.packets[j].isSelected = false;
                plantSelector.packets[j].sprite->setColor(sf::Color::White);
                break;
              }
            }
            chosenPlants.erase([removedId](PlantType& p) {
              return (int)p == removedId;
              });

            delete plantSelector.selectedSlot[i].sprite;

            for (int j = i; j < 6; j++) {
              plantSelector.selectedSlot[j].active = plantSelector.selectedSlot[j + 1].active;
              plantSelector.selectedSlot[j].id = plantSelector.selectedSlot[j + 1].id;
              plantSelector.selectedSlot[j].sprite = plantSelector.selectedSlot[j + 1].sprite;

              if (plantSelector.selectedSlot[j].active && plantSelector.selectedSlot[j].sprite) {
                float startX = 90.0f;
                float gapX = 59.0f;
                plantSelector.selectedSlot[j].sprite->setPosition({ startX + (j * gapX), 11.0f });
              }
            }

            plantSelector.selectedSlot[6].sprite = nullptr;
            plantSelector.selectedSlot[6].active = false;

            plantSelector.currentSelectedCnt--;

            break;
          }
        }
      }
    }
  }
  mousePressedLastFrame = isLeftPressed;
  if (plantSelector.isSlidingOut && plantSelector.currX <= -680.f) {
    plantSelector.isVisible = false;
  }
}

  void PlantsSelector::drawSelector(sf::RenderWindow& window) {
    sf::View oldView = window.getView();
    window.setView(*view);

    if (plantSelector.isVisible) {
      if (plantSelector.backSprite) window.draw(*plantSelector.backSprite);
      for (int i = 0; i < packetsNum; i++) {
        if (plantSelector.packets[i].sprite)
          window.draw(*plantSelector.packets[i].sprite);
      }
      if (plantSelector.playBtn) window.draw(*plantSelector.playBtn);
    }

  
    for (int i = 0; i < 7; i++) {
      if (plantSelector.selectedSlot[i].active && plantSelector.selectedSlot[i].sprite) {
        float baseStartX = 90.0f; 
        float gapX = 59.0f;
        float packetScale = 1.15f;

        plantSelector.selectedSlot[i].sprite->setScale({ packetScale,packetScale });
        plantSelector.selectedSlot[i].sprite->setPosition({ baseStartX + (i * gapX), 11.0f });

        window.draw(*plantSelector.selectedSlot[i].sprite);
      }
    }

    window.setView(oldView);
  }


