#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

extern bool isPaused;
extern int gameState;
extern int homeState;
extern float deltaTime;

void onClick(sf::Text &,
             std::function<void()>); // Execute an action when button is clicked
float randomRange(float x, float y); // Return random value from x to y
