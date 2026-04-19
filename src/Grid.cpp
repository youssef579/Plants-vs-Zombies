#include <Grid.hpp>
#include <Packets/Packet.hpp>
#include <Packets/Shovel.hpp>
#include <SunManager.hpp>

Cell grid[ROWS_NUMBER][COLUMNS_NUMER];

void initGrid(){
  float y = 104;
  for (int i = 0; i < ROWS_NUMBER; i++){
    float x = 131;
    for (int j = 0; j < COLUMNS_NUMER; j++){
      grid[i][j].rectangle.setSize({columnLenth[j], rowLenth[i]});
      grid[i][j].rectangle.setOrigin(grid[i][j].rectangle.getLocalBounds().size / 2.0f);
      grid[i][j].rectangle.setPosition({x + columnLenth[j] / 2, y + rowLenth[i] / 2});
      grid[i][j].rectangle.setFillColor(sf::Color({0, 0, 0, 0}));
      grid[i][j].plantPosition = {x + columnLenth[j] / 2 + 2, y + rowLenth[i] / 2 - 10};
      grid[i][j].therePlantInBounders = 0;
      x += columnLenth[j];
    }
    y += rowLenth[i];
  }
}

void updateGrid(float dt){
  for (int i = 0; i < ROWS_NUMBER; i++){
      for (int j = 0; j < COLUMNS_NUMER; j++){
          if (grid[i][j].plant.has_value()){

            grid[i][j].plant.value().update(dt);

            if (grid[i][j].plant.has_value() && shovel.selected && grid[i][j].rectangle.getGlobalBounds().contains(mousePosition) && !grid[i][j].therePlantInBounders){
              shovel.movingShovel->setColor(sf::Color{125, 125, 125, 230});
              grid[i][j].therePlantInBounders = 1;
            }

            if (grid[i][j].plant.has_value() && shovel.selected && !grid[i][j].rectangle.getGlobalBounds().contains(mousePosition) && grid[i][j].therePlantInBounders){
              shovel.movingShovel->setColor(sf::Color{255, 255, 255, 255});
              grid[i][j].therePlantInBounders = 0;
            }
              
            if (grid[i][j].plant.has_value() && shovel.selected && grid[i][j].rectangle.getGlobalBounds().contains(mousePosition) && isMouseReleased)
              grid[i][j].plant.reset();
  
            if (grid[i][j].plant.has_value() && grid[i][j].plant.value().health <= 0)
              grid[i][j].plant.reset();

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
                    grid[i][j].plant = Plant(SUN_FLOWER, grid[i][j].plantPosition, i, ReAnimator::getDefinition(REANIM_SUNFLOWER));
                    break;
                  case WALLNUT:
                    grid[i][j].plant = Plant(WALLNUT, grid[i][j].plantPosition, i, ReAnimator::getDefinition(REANIM_WALLNUT));
                    break;
                  case PEASHOOTER:
                    grid[i][j].plant = Plant(PEASHOOTER, grid[i][j].plantPosition, i, ReAnimator::getDefinition(REANIM_PEASHOOTER));
                    break;
                  case SNOWPEASHOOTER:
                    grid[i][j].plant = Plant(SNOWPEASHOOTER, grid[i][j].plantPosition, i, ReAnimator::getDefinition(REANIM_SNOWPEA));
                    break;
                  case REPEATERPEA:
                    grid[i][j].plant = Plant(REPEATERPEA, grid[i][j].plantPosition, i, ReAnimator::getDefinition(REANIM_REPEATER));
                    break;
                  default:
                    break;
                }

                packets[k].reloadTimer = packets[k].reloadDuration;
                Sun::sunBalance -= packets[k].cost;
                grid[i][j].therePlantInBounders = 0;
                
              } 

            }
          }
      }
  }
}

void drawGrid(){
  for (int i = 0; i < ROWS_NUMBER; i++){
    for (int j = 0; j < COLUMNS_NUMER; j++){
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
}