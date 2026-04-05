#pragma once

#include <SFML/Graphics.hpp>
#include <Weather.hpp>

extern sf::RenderWindow *window;
extern const sf::Vector2u WINDOW_SIZE;

void initWindow();
void setCursorMain();
void setCursorHover();

void handleEvents();