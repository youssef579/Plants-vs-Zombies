#include <AssetsManager.hpp>
#include <Home.hpp>
#include <SFML/Graphics.hpp>
#include <functional>
#include <globals.hpp>
#include <Overlay.hpp>

int homeState = 0;

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

void updateHome() {
  static sf::Texture &backgroundTexture = getTexture("assets/home.png");
  static sf::Texture &headerTexture = getTexture("assets/home_header.png");

  static sf::Sprite backgroundSprite(backgroundTexture);
  static sf::Sprite headerSprite(headerTexture);

  static sf::Text playButton(assets->font, "Play", 40);
  static sf::Text creditsButton(assets->font, "Credits", 40);
  static sf::Text exitButton(assets->font, "Exit", 40);

  static bool isInit = false;
  if (!isInit) {
    headerSprite.setPosition(
        {(window->getSize().x - headerTexture.getSize().x) / 2.0f, 20});

    playButton.setPosition({920, 420});
    creditsButton.setPosition({860, 470});
    exitButton.setPosition({920, 520});
    sf::Mouse::setPosition({0, 0}, *window);

    isInit = true;
  }
  
  if (homeState == 0){
    // onClick(playButton, mousePosition);
    onClick(exitButton, []() { window->close(); });
  
    onClick(creditsButton, [&]() {
      homeState = 1;
      std::string credits[7] = {"Anton", "Youssef", "Ali", "Ather", "Halimo", "Mohamed", "MSoliman"};
      update_overlay(7, credits, "Credits");
    });
  }

  window->draw(backgroundSprite);
  window->draw(headerSprite);
  window->draw(playButton);
  window->draw(creditsButton);
  window->draw(exitButton);

  if (homeState == 1) {
    onClick(*overlay->OK, [&]() {homeState = 0;});
    print_overlay();
  }
}
