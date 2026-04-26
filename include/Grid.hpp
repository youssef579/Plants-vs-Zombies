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
const float columnLenth[9] = {108.5, 99.5, 113.4, 104.1, 103.5, 105.85, 98.3, 109.5, 120};

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

void clearGrid();

sf::Vector2i positionToGrid(sf::Vector2f pos);
