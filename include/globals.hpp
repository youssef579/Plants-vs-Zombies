#pragma once

#include <SFML/Graphics.hpp>
#include <functional>


extern bool isPaused;
extern bool isMousePressed, isMouseReleased;
extern int gameState;
extern int homeState;

void onClick(sf::Text &, std::function<void()>);

int randomRange(int x, int y);       // Return random int from x to y
float randomRange(float x, float y); // Return random float from x to y
