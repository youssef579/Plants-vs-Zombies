#include <Home.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <globals.hpp>
#include <iostream>
#include <ostream>

sf::Texture backgroundTexture("assets/home.png");
sf::Sprite backgroundSprite(backgroundTexture);

sf::Texture headerTexture("assets/home_header.png");
sf::Sprite headerSprite(headerTexture);

sf::Text startButton(font, "Play", 50);
sf::Text exitButton(font, "Exit", 50);

void hoverOnButton(sf::Text &button, sf::Vector2f &mousePosition) {
  if (button.getGlobalBounds().contains(mousePosition))
    button.setStyle(sf::Text::Underlined);
  else
    button.setStyle(sf::Text::Regular);
}

void updateHome() {
  static bool init = false;

  if (!init) {
    headerSprite.setPosition(
        {(window.getSize().x - headerTexture.getSize().x) / 2.0f, 20});

    exitButton.setPosition({920, 520});
    startButton.setPosition({920, 460});

    init = true;
  }
  sf::Vector2f mousePosition =
      window.mapPixelToCoords(sf::Mouse::getPosition(window));

  hoverOnButton(startButton, mousePosition);
  hoverOnButton(exitButton, mousePosition);

  static bool canClick = false;
  bool isPressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

  if (!isPressed)
    canClick = true;

  if (isPressed && canClick) {
    if (exitButton.getGlobalBounds().contains(mousePosition))
      window.close();
    canClick = false;
  }

  window.draw(backgroundSprite);
  window.draw(headerSprite);
  window.draw(exitButton);
  window.draw(startButton);
}
