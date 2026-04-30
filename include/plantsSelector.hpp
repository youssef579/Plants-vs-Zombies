#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>


extern sf::Texture& getTexture(std::string path);

struct selectionPacket {
  sf::Sprite* sprite = nullptr;
  sf::Vector2f originalPos; 
  std::string plantName;
  bool isDragging = false;
  bool isChosen = false;

  void init(std::string name, float x, float y);
};

struct PlantSelector {
  sf::Sprite* background = nullptr;
  sf::Sprite* startButton = nullptr;
  std::vector<selectionPacket*> inventory; 
  std::vector<sf::FloatRect> baskets;

  selectionPacket* draggedPacket = nullptr;
  int selectedCount = 0;

  void setup();
  void update(sf::RenderWindow& window);
  void draw(sf::RenderWindow& window);
  bool isStartBtnPressed(sf::RenderWindow& window);
};
