#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

#include <PvP/Peer.hpp>

extern bool isPaused;
extern bool isMousePressed, isMouseReleased;
extern sf::Vector2f mousePosition;
extern int gameState;
extern int homeState;

void onClick(sf::Text &, std::function<void()>);
void onClick(sf::Sprite &button, std::function<void()> actionClick,
  std::function<void()> actionHover); // sprite click detection
void onClick(sf::FloatRect rect, float rot, std::function<void()> actionClick,
  std::function<void()> actionHover); // rect click detection

int randomRange(int x, int y);       // Return random int from x to y
float randomRange(float x, float y); // Return random float from x to y

extern bool plants;
extern unsigned short ports[2];