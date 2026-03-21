#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

using TextStyle = std::function<void(sf::Text&)>;

const int MAX_LINES = 16;

struct Overlay {
  int numberOfLines = 0;
  sf::RectangleShape *overlaycolor = nullptr;
  sf::Text *Title = nullptr, *strings[MAX_LINES]{}, *OK = nullptr;
};

extern Overlay *overlay;

void updateOverlay(const int &numberStrings, std::string arr[], const std::string &title, TextStyle changeText = nullptr);

void printOverlay();