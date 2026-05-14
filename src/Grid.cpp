#include <Grid.hpp>
#include <Packets/Packet.hpp>
#include <Packets/Shovel.hpp>
#include <SunManager.hpp>
#include <BackgroundManager.hpp>
#include <LevelManager.hpp>

Cell grid[ROWS_NUMBER][COLUMNS_NUMBER];

void initGrid(){
  float y = 120;
  const float offsetY = -12, offsetX = 0;

  for (int i = 0; i < ROWS_NUMBER; i++){
    float x = 134;
    for (int j = 0; j < COLUMNS_NUMBER; j++){
      float columnLenth = dayColumnLenth[j];

      if(levelManager.levels[levelManager.currentLevel - 1]->location == LevelManager::Level::Night)
        columnLenth = nightColumnLenth[j];  // change it by type of level in level manager
      grid[i][j].rectangle.setSize({columnLenth, rowLenth[i]});
      grid[i][j].rectangle.setOrigin(grid[i][j].rectangle.getLocalBounds().size / 2.0f);
      grid[i][j].rectangle.setPosition({x + columnLenth / 2, y + rowLenth[i] / 2});
      grid[i][j].rectangle.setFillColor(sf::Color({0, 0, 0, 0}));
      //grid[i][j].rectangle.setOutlineColor(sf::Color::Black);
      //grid[i][j].rectangle.setOutlineThickness(1);
      grid[i][j].plantPosition = {x + columnLenth / 2 + offsetX, y + rowLenth[i] / 2 + offsetY};
      grid[i][j].therePlantInBounders = 0;
      x += columnLenth;
    }
    y += rowLenth[i];
  }
}

void updateGrid(float dt){
  bool isShovelDark = 0;
  for (int i = 0; i < ROWS_NUMBER; i++){
      for (int j = 0; j < COLUMNS_NUMBER; j++){
          if (grid[i][j].plant.has_value()){

            grid[i][j].plant.value().update(dt);

            if (grid[i][j].plant.has_value() && grid[i][j].plant.value().health <= 0)
              // Removed because of death
              grid[i][j].plant.reset();

            if (grid[i][j].plant.has_value() && shovel.selected && grid[i][j].rectangle.getGlobalBounds().contains(mousePosition) && isMouseReleased) {
              // Removed using shovel
              grid[i][j].plant.reset();
              sounds.play("Shovel");
            }

              
            if (grid[i][j].plant.has_value() && shovel.selected && grid[i][j].rectangle.getGlobalBounds().contains(mousePosition) && !grid[i][j].therePlantInBounders){
              isShovelDark = 1;
            }

          }else{
            for (int k = 0; k < packets.size; k++){

              if (packets[k].selected && grid[i][j].rectangle.getGlobalBounds().contains(mousePosition) && !grid[i][j].therePlantInBounders){
                packets[k].plantShadow.setPosition(grid[i][j].plantPosition);
                grid[i][j].therePlantInBounders = 1;
              }

              if (packets[k].selected && !grid[i][j].rectangle.getGlobalBounds().contains(mousePosition) && grid[i][j].therePlantInBounders)
                grid[i][j].therePlantInBounders = 0;


              if (packets[k].selected && grid[i][j].rectangle.getGlobalBounds().contains(mousePosition) && isMouseReleased){
                switch (packets[k].plantType){
                  case SUN_FLOWER:
                    grid[i][j].plant = Plant(SUN_FLOWER, grid[i][j].plantPosition, i, j, ReAnimator::getDefinition(REANIM_SUNFLOWER));
                    break;
                  case WALLNUT:
                    grid[i][j].plant = Plant(WALLNUT, grid[i][j].plantPosition, i, j, ReAnimator::getDefinition(REANIM_WALLNUT));
                    break;
                  case PEASHOOTER:
                    grid[i][j].plant = Plant(PEASHOOTER, grid[i][j].plantPosition, i, j, ReAnimator::getDefinition(REANIM_PEASHOOTER));
                    break;
                  case SNOWPEASHOOTER:
                    grid[i][j].plant = Plant(SNOWPEASHOOTER, grid[i][j].plantPosition, i, j, ReAnimator::getDefinition(REANIM_SNOWPEA));
                    break;
                  case REPEATERPEA:
                    grid[i][j].plant = Plant(REPEATERPEA, grid[i][j].plantPosition, i, j, ReAnimator::getDefinition(REANIM_REPEATER));
                    break;
                  case TALLNUT:
                    grid[i][j].plant = Plant(TALLNUT, grid[i][j].plantPosition, i, j, ReAnimator::getDefinition(REANIM_TALLNUT));
                    break;
                  case CHERRYBOMB:
                    grid[i][j].plant = Plant(CHERRYBOMB, grid[i][j].plantPosition, i, j, ReAnimator::getDefinition(REANIM_CHERRYBOMB));
                    break;
                  case JALAPENO:
                    grid[i][j].plant = Plant(JALAPENO, grid[i][j].plantPosition, i, j, ReAnimator::getDefinition(REANIM_JALAPENO));
                    break;
                  case POTATOMINE:
                    grid[i][j].plant = Plant(POTATOMINE, grid[i][j].plantPosition, i, j, ReAnimator::getDefinition(REANIM_POTATO_MINE));
                    break;
                  case ICESHROOM:
                    grid[i][j].plant = Plant(ICESHROOM, grid[i][j].plantPosition, i, j, ReAnimator::getDefinition(REANIM_ICESHROOM));
                    break;
                  case SQUASH:
                    grid[i][j].plant = Plant(SQUASH, grid[i][j].plantPosition, i, j, ReAnimator::getDefinition(REANIM_SQUASH));
                    break;
                  case PUFFSHROOM:
                    grid[i][j].plant = Plant(PUFFSHROOM, grid[i][j].plantPosition, i, j, ReAnimator::getDefinition(REANIM_PUFFSHROOM));
                    break;
                  default:
                    break;
                }

                packets[k].reloadTimer = packets[k].reloadDuration;
                Sun::sunBalance -= packets[k].cost;
                grid[i][j].therePlantInBounders = 0;
                sounds.play((rand() & 1) ? "Plant1" : "Plant2");
              
              } 

            }
          }
      }
  }

  if (isShovelDark){
    if (shovel.movingShovel->getColor() != sf::Color{125, 125, 125, 230})
      shovel.movingShovel->setColor(sf::Color{125, 125, 125, 230});
  }else{
    if (shovel.movingShovel->getColor() != sf::Color{255, 255, 255, 255})
      shovel.movingShovel->setColor(sf::Color{255, 255, 255, 255});
  }
  
}

void drawGrid(){
  window->setView(*gameView);
  for (int i = 0; i < ROWS_NUMBER; i++){
    for (int j = 0; j < COLUMNS_NUMBER; j++){
      window->draw(grid[i][j].rectangle);
      if (grid[i][j].plant.has_value()){

        grid[i][j].plant.value().draw();

      }else{
        for (int k = 0; k < packets.size; k++){

          if (packets[k].selected && grid[i][j].rectangle.getGlobalBounds().contains(mousePosition))
            window->draw(packets[k].plantShadow);

        }
      }
    }
  }
  window->setView(*view);
}

void clearGrid() {
  for (int i = 0; i < ROWS_NUMBER; i++) {
    for (int j = 0; j < COLUMNS_NUMBER; j++) {
      if (grid[i][j].plant.has_value())
        grid[i][j].plant.value().health = 0;
      }
  }
}

sf::Vector2i positionToGrid(sf::Vector2f pos) {
  for (int i = 0; i < ROWS_NUMBER; i++) {
    for (int j = 0; j < COLUMNS_NUMBER; j++) {
      if (grid[i][j].rectangle.getGlobalBounds().contains(pos))
        return { i, j };
    }
  }
  return { -1, -1 }; // position out of grid bounds
}
