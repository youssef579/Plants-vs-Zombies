#pragma once

#include <SFML/Graphics.hpp>
#include <Weather.hpp>

extern sf::RenderWindow *window;
extern sf::View *view;
extern const sf::Vector2u WINDOW_SIZE;

void initWindow();

void setWindowMetaData();

void setCursorMain();

void setCursorHover();

void getLetterboxView(int windowWidth, int windowHeight);

void handleEvents();
