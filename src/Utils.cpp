#include <Window.hpp>
#include <globals.hpp>
#include <random>

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

float randomRange(float x, float y) { // random value from x to y
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dist(x, y);
  return dist(gen);
}
