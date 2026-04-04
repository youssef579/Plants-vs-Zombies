#include <Game.hpp>
#include <Home.hpp>
#include <Level.hpp>
#include <globals.hpp>

int gameState = 0;
/*
  0 -> Home menu
  otherwise -> Current level
*/

bool isPaused = false;

void updateGame() {
  switch (gameState) {
  case 0:
    updateHome();
    break;
  default:
    updateLevel();
    break;
  }
}
