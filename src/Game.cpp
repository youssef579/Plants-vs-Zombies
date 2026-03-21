#include <Game.hpp>
#include <Home.hpp>
#include <Overlay.hpp>

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
