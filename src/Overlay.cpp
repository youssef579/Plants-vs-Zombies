#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <AssetsManager.hpp>
#include <Game.hpp>
#include <globals.hpp>
#include <Overlay.hpp>
#include <Home.hpp>
#include <iostream>
#include <functional>

using TextStyle = std::function<void(sf::Text&)>;

Overlay *overlay = nullptr;

void updateOverlay(const int &nLines, std::string arr[], const std::string &title, TextStyle changeText){
  if (overlay == nullptr) {
    overlay = new Overlay();
  }

  overlay->numberOfLines = nLines;

  if (overlay->overlaycolor == nullptr) {
    overlay->overlaycolor = new sf::RectangleShape({1150, 606});
    overlay->overlaycolor->setFillColor(sf::Color({0, 0, 0, 200}));
  }
  
  int font_size = 40, title_size = 55;
  float space = 0;
  
  overlay->Title = new sf::Text(assets->font, title, title_size);
  space += overlay->Title->getLocalBounds().size.y + 35;
  
  for (int i = 0; i < nLines; i++){
    overlay->strings[i] = new sf::Text(assets->font, arr[i], font_size);
    if (changeText) {
      changeText(*overlay->strings[i]);
    }
    space += overlay->strings[i]->getLocalBounds().size.y + 15;
  }

  overlay->OK = new sf::Text(assets->font, "OK", title_size);
  space += overlay->OK->getLocalBounds().size.y + 20;

  float y = (606 - space) / 2;

  overlay->Title->setPosition({1150 / 2 - (overlay->Title->getLocalBounds().size.x / 2), y});
  y += overlay->Title->getLocalBounds().size.y + 35;
  
  for (int i = 0; i < nLines; i++){
    overlay->strings[i]->setPosition({1150 / 2 - (overlay->strings[i]->getLocalBounds().size.x / 2), y});
    y += overlay->strings[i]->getLocalBounds().size.y + 15;
  }
  y += 20;

  overlay->OK->setPosition({1150 / 2 - (overlay->OK->getLocalBounds().size.x / 2), y});
  overlay->OK->setFillColor(sf::Color::Green);
}

void printOverlay(){
  window->draw(*overlay->overlaycolor);
  window->draw(*overlay->Title);
  for (int i = 0; i < overlay->numberOfLines; i++){
    window->draw(*overlay->strings[i]);
  }
  window->draw(*overlay->OK);
}