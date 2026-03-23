#pragma once

#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>


void initFiles();
void createLevelSelectorVars();
void loadLevelsFile();
void updateFiles();

//level selector
const int LEVEL_SELECTOR_MAX_PAGES = 10;
const int LEVEL_SELECTOR_MAX_LEVELS_PER_PAGE = 5;
const sf::Color UNLOCKED_LEVEL_COLOR = { 255, 255, 255, 255 };
const sf::Color LOCKED_LEVEL_COLOR = { 120, 120, 120, 255 };
extern std::string levelSelectorNames[LEVEL_SELECTOR_MAX_PAGES][LEVEL_SELECTOR_MAX_LEVELS_PER_PAGE];
extern sf::Color levelSelectorColors[LEVEL_SELECTOR_MAX_PAGES][LEVEL_SELECTOR_MAX_LEVELS_PER_PAGE];
extern int levelSelectorCurrentPage;