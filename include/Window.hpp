#pragma once

#include <SFML/Graphics.hpp>

extern sf::RenderWindow *window;

void initWindow();
void setCursorMain();
void setCursorHover();

void handleEvents();
