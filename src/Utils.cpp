#include <Window.hpp>
#include <globals.hpp>

void onClick(sf::Text &button, std::function<void()> action) {
  if (button.getGlobalBounds().contains(mousePosition)) {
    button.setStyle(sf::Text::Bold);

    if (isMousePressed)
      action();
  } else
    button.setStyle(sf::Text::Regular);
}

int randomRange(int x, int y) {        // random int from x to y
  return x + rand() % (y - x + 1);
}

float randomRange(float x, float y) {  // random float from x to y
  return x + (rand()) / ( (float)(RAND_MAX / (y - x)));
}
