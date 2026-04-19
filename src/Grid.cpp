#include <Grid.hpp>

Cell grid[ROWS_NUMBER][COLUMNS_NUMER];

void initGrid(){
  float y = 104;
  for (int i = 0; i < ROWS_NUMBER; i++){
    float x = 136;
    for (int j = 0; j < COLUMNS_NUMER; j++){
      grid[i][j].rectangle.setSize({columnLenth[j], rowLenth[i]});
      grid[i][j].rectangle.setOrigin(grid[i][j].rectangle.getLocalBounds().size / 2.0f);
      grid[i][j].rectangle.setPosition({x + columnLenth[j] / 2, y + rowLenth[i] / 2});
      grid[i][j].rectangle.setFillColor(sf::Color::Yellow);
      grid[i][j].therePlantInBounders = 0;
      x += columnLenth[j];
    }
    y += rowLenth[i];
  }
}

void drawGrid(){
  for (int i = 0; i < ROWS_NUMBER; i++){
    for (int j = 0; j < COLUMNS_NUMER; j++){
      window->draw(grid[i][j].rectangle);
    }
  }
}