#include <Window.hpp>
#include <globals.hpp>

void onClick(sf::Text &button, std::function<void()> action) {
  sf::Vector2f mousePosition =
      window->mapPixelToCoords(sf::Mouse::getPosition(*window));

  if (button.getGlobalBounds().contains(mousePosition)) {
    button.setStyle(sf::Text::Bold);
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
      action();
  } else
    button.setStyle(sf::Text::Regular);
}
