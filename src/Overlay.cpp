#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <AssetsManager.hpp>
#include <Game.hpp>
#include <globals.hpp>
#include <Overlay.hpp>
#include <Home.hpp>
#include <iostream>

Overlay *overlay = nullptr;

void update_overlay(const int &sz, std::string arr[], const std::string &header){
  if (overlay == nullptr) {
    overlay = new Overlay();
  }

  overlay->number_of_strings = sz;

  if (overlay->overlaycolor == nullptr) {
    overlay->overlaycolor = new sf::RectangleShape({1150, 606});
    overlay->overlaycolor->setFillColor(sf::Color({0, 0, 0, 200}));
  }
  
  int font_sz = 40, header_sz = 55;
  float space = 0;
  
  overlay->Header = new sf::Text(assets->font, header, header_sz);
  space += overlay->Header->getLocalBounds().size.y + 35;
  
  for (int i = 0; i < sz; i++){
    overlay->strings[i] = new sf::Text(assets->font, arr[i], font_sz);
    space += overlay->strings[i]->getLocalBounds().size.y + 15;
  }

  overlay->OK = new sf::Text(assets->font, "OK", header_sz);
  space += overlay->OK->getLocalBounds().size.y + 20;

  float y = (606 - space) / 2;

  overlay->Header->setPosition({1150 / 2 - (overlay->Header->getLocalBounds().size.x / 2), y});
  y += overlay->Header->getLocalBounds().size.y + 35;
  
  for (int i = 0; i < sz; i++){
    overlay->strings[i]->setPosition({1150 / 2 - (overlay->strings[i]->getLocalBounds().size.x / 2), y});
    y += overlay->strings[i]->getLocalBounds().size.y + 15;
  }
  y += 20;
  overlay->OK->setPosition({1150 / 2 - (overlay->OK->getLocalBounds().size.x / 2), y});
  overlay->OK->setFillColor(sf::Color::Green);
}

void print_overlay(){
  window->draw(*overlay->overlaycolor);
  window->draw(*overlay->Header);
  for (int i = 0; i < overlay->number_of_strings; i++){
    window->draw(*overlay->strings[i]);
  }
  window->draw(*overlay->OK);
}