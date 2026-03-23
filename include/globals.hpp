#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

extern sf::RenderWindow *window;
extern bool buttonWasClicked;
extern int maxLevelUnlocked;
void onClick(sf::Text&, std::function<void()>);

const int MAX_LEVELS = 18;
