#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

const int MAX_LINES = 7;

extern bool isOverlayChanged;

struct Overlay {
  int nLines;
  sf::RectangleShape overlayRect;
  sf::Text title, greenButton, redButton;
  std::optional<sf::Text> lines[MAX_LINES];
};

extern Overlay *overlay;

void initOverlay();

void handleOverlayEvents(
    std::function<void(sf::Text &, int)> updateLine = nullptr,
    std::function<void()> greenButtonAction = nullptr,
    std::function<void()> redButtonAction = nullptr);

void setOverlayData(int nLines, std::string title,
                    std::function<void(sf::Text &, int)> setupLine,
                    std::string greenButtonText = "",
                    std::string redButtonText = "");

void drawOverlay();
