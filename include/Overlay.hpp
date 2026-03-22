#pragma once

#include <AssetsManager.hpp>
#include <SFML/Graphics.hpp>
#include <functional>
#include <globals.hpp>
#include <vector>

const int MAX_LINES = 7;

extern bool isOverlayChanged;

struct Overlay {
  int nLines;
  sf::RectangleShape overlayRect;
  sf::Text title, greenButton, redButton;
  std::vector<sf::Text> lines;
};

extern Overlay *overlay;

void initOverlay();

void updateOverlay(int, std::string[], std::string,
                   std::function<void(sf::Text &)> = nullptr,
                   std::function<void()> = nullptr, std::string = "",
                   std::function<void()> = nullptr, std::string = "");

void drawOverlay();
