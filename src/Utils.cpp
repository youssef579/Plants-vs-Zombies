#include <Window.hpp>
#include <globals.hpp>

void onClick(sf::Text &button, std::function<void()> action) {
  static bool wasButtonClicked = false;
  sf::Vector2f mousePosition =
      window->mapPixelToCoords(sf::Mouse::getPosition(*window));

  if (button.getGlobalBounds().contains(mousePosition)) {
    button.setStyle(sf::Text::Bold);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
      if (!wasButtonClicked) {
        action();
        wasButtonClicked = true;
      }
    } else
      wasButtonClicked = false;

  } else
    button.setStyle(sf::Text::Regular);
}

int randomRange(int x, int y) {        // random int from x to y
  return x + rand() % (y - x + 1);
}

float randomRange(float x, float y) {  // random float from x to y
  return x + (rand()) / ( (float)(RAND_MAX / (y - x)));
}
