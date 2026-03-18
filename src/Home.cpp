#include <Home.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <globals.hpp>

sf::Texture backgroundTexture("assets/home.png");
sf::Sprite backgroundSprite(backgroundTexture);

sf::Texture headerTexture("assets/home_header.png");
sf::Sprite headerSprite(headerTexture);

sf::Text playButton(font, "Play", 40);
sf::Text creditsButton(font, "Credits", 40);
sf::Text exitButton(font, "Exit", 40);

void hoverOnButton(sf::Text &button, sf::Vector2f &mousePosition) {
  if (button.getGlobalBounds().contains(mousePosition))
    button.setStyle(sf::Text::Bold);
  else
    button.setStyle(sf::Text::Regular);
}

void updateHome() {
  static bool init = false;
  if (!init) {
    headerSprite.setPosition(
        {(window.getSize().x - headerTexture.getSize().x) / 2.0f, 20});

    playButton.setPosition({920, 420});
    creditsButton.setPosition({860, 470});
    exitButton.setPosition({920, 520});

    init = true;
  }

  sf::Vector2f mousePosition =
      window.mapPixelToCoords(sf::Mouse::getPosition(window));

  hoverOnButton(playButton, mousePosition);
  hoverOnButton(exitButton, mousePosition);
  hoverOnButton(creditsButton, mousePosition);

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
  window.draw(playButton);
  window.draw(creditsButton);
}
