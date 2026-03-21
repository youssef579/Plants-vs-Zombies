#pragma once

#include <SFML/Graphics.hpp>

const int mx_size_in_overlay = 16;

struct Overlay {
  int number_of_strings = 0;
  sf::RectangleShape *overlaycolor = nullptr;
  sf::Text *Header = nullptr, *strings[mx_size_in_overlay]{}, *OK = nullptr;
};

extern Overlay *overlay;

void update_overlay(const int &sz, std::string arr[], const std::string &header);

void print_overlay();