#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

extern sf::RenderWindow *window;

void onClick(sf::Text &, std::function<void()>);
