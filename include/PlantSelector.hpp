#pragma once

#include <SFML/Graphics.hpp>
#include <Plants/Plant.hpp>


extern Array<PlantType> chosenPlants;
const int packetsNum = 12;
extern bool isPlantUnlocked[packetsNum];

void initGameProgress();

struct SelectionPacket {
  int id;
  sf::Sprite* sprite = nullptr;
  bool isSelected = false;
};

struct selectedPacket {
  int id;
  sf::Sprite* sprite = nullptr;
  bool active = false; 
};

struct PlantsSelector {

  sf::Sprite* backSprite = nullptr;
  sf::Sprite* playBtn = nullptr;

  sf::Texture playBtnNormal;
  sf::Texture playBtnHover;

  sf::Text* playText = nullptr;

  float currX;
  float tarX;
  float speed;
  bool isVisible;
  bool isSlidingOut;
  

  SelectionPacket packets[packetsNum];
  selectedPacket selectedSlot[7];
  int currentSelectedCnt = 0;

  void initSelector();
  void slideIn();
  void slideOut();
  void updateSelector(float dt, sf::RenderWindow& window);
  void drawSelector(sf::RenderWindow& window);
};

extern PlantsSelector plantSelector;


sf::Vector2f getPacketPosition(int index);
