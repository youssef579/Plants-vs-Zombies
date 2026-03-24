#include <Window.hpp>
#include <globals.hpp>
void onClick(sf::Text& button, std::function<void()> action) {
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
    }
    else
      wasButtonClicked = false;

  }
  else
    button.setStyle(sf::Text::Regular);
}

bool onClickSun(Sun*& sun, std::function<void(Sun* s)> action) {
  static bool wasButtonClicked = false;
  static bool hovering = false;
  sf::Vector2f mousePosition =
    window->mapPixelToCoords(sf::Mouse::getPosition(*window));

  if (sun->sprite.getGlobalBounds().contains(mousePosition)) {
    hovering = true;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
      if (!wasButtonClicked) {
        action(sun);
        wasButtonClicked = true;
      }
    }
    else
      wasButtonClicked = false;

   return true;

  }
  return false;

}