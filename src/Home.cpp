#include <AssetsManager.hpp>
#include <Home.hpp>
#include <SFML/Graphics.hpp>
#include <globals.hpp>

void hoverOnButton(sf::Text &button, sf::Vector2f &mousePosition) {
  if (button.getGlobalBounds().contains(mousePosition))
    button.setStyle(sf::Text::Bold);
  else
    button.setStyle(sf::Text::Regular);
}

void updateHome() {
  static sf::Texture &backgroundTexture = getTexture("assets/home.png");
  static sf::Texture &headerTexture = getTexture("assets/home_header.png");

  static sf::Sprite backgroundSprite(backgroundTexture);
  static sf::Sprite headerSprite(headerTexture);

  static sf::Text playButton(assets->font, "Play", 40);
  static sf::Text creditsButton(assets->font, "Credits", 40);
  static sf::Text exitButton(assets->font, "Exit", 40);

  static bool init = false;
  if (!init) {
    headerSprite.setPosition(
        {(window->getSize().x - headerTexture.getSize().x) / 2.0f, 20});

    playButton.setPosition({920, 420});
    creditsButton.setPosition({860, 470});
    exitButton.setPosition({920, 520});

    init = true;
  }

  sf::Vector2f mousePosition =
      window->mapPixelToCoords(sf::Mouse::getPosition(*window));

  hoverOnButton(playButton, mousePosition);
  hoverOnButton(exitButton, mousePosition);
  hoverOnButton(creditsButton, mousePosition);

  static bool canClick = false;
  bool isPressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

  if (!isPressed)
    canClick = true;

  if (isPressed && canClick) {
    if (exitButton.getGlobalBounds().contains(mousePosition))
      window->close();
    canClick = false;
  }

  window->draw(backgroundSprite);
  window->draw(headerSprite);
  window->draw(exitButton);
  window->draw(playButton);
  window->draw(creditsButton);
}
