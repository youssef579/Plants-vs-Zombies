#include <globals.hpp>

void onClick(sf::Text& button, std::function<void()> action) {
  sf::Vector2f mousePosition =
      window->mapPixelToCoords(sf::Mouse::getPosition(*window));

  if (button.getGlobalBounds().contains(mousePosition)) {
    button.setStyle(sf::Text::Bold);
    /*if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
      action();*/
    
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
      if (!buttonWasClicked) {
        action();
        buttonWasClicked = true;
      }
    }
    else buttonWasClicked = false;
    

  }
  else {
    button.setStyle(sf::Text::Regular);
  }
}
