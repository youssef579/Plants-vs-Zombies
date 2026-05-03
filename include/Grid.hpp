#pragma once
#include <globals.hpp>
#include <Window.hpp>
#include <AssetsManager.hpp>
#include <Audio.hpp>
#include <Animation.hpp>
#include <ReAnimation/ReAnimation.hpp>
#include <Plants/Plant.hpp>
#include <functional>

const int ROWS_NUMBER = 5, COLUMNS_NUMBER = 9;
const float rowLenth[5] = {98.3, 85, 96.4, 94.5, 98.3};
const float dayColumnLenth[9] = {108.5, 99.5, 110, 104.1, 103.5, 103.85, 97.3, 110, 116};
const float nightColumnLenth[9] = {108.5, 99.5, 110.4, 104.1, 101.5, 100.85, 92, 102, 118};

struct Cell {
  sf::RectangleShape rectangle;
  bool therePlantInBounders;
  sf::Vector2f plantPosition;
  std::optional < Plant > plant;
};

extern Cell grid[ROWS_NUMBER][COLUMNS_NUMBER];

void initGrid();

void updateGrid(float dt);

void drawGrid();

sf::Vector2i positionToGrid(sf::Vector2f pos);
