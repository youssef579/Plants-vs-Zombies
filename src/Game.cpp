#include <Game.hpp>
#include <Home.hpp>

int gameState = 0;
/*
  0 -> Home menu
*/

void updateGame() {
  switch (gameState) {
  case 0:
    updateHome();
    break;
  }
}
