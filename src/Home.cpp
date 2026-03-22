#include <AssetsManager.hpp>
#include <Home.hpp>
#include <Overlay.hpp>
#include <SFML/Graphics.hpp>
#include <functional>
#include <globals.hpp>

std::string names[] = {"Youssef Ragaey (Team Lead)",
                       "Anton Bakhet",
                       "Ali Assem",
                       "Mohammed Abdelhalim",
                       "Mohammed Ahmed",
                       "Ather Hossam",
                       "Mohammed Soliman"};

void updateHome() {
  static sf::Texture &backgroundTexture = getTexture("assets/home.png");
  static sf::Texture &headerTexture = getTexture("assets/home_header.png");

  static sf::Sprite backgroundSprite(backgroundTexture);
  static sf::Sprite headerSprite(headerTexture);

  static sf::Text playButton(assets->font, "Play", 40);
  static sf::Text creditsButton(assets->font, "Credits", 40);
  static sf::Text exitButton(assets->font, "Exit", 40);

  static int homeState = 0;
  /*
    0 -> Home menu
    1 -> Credits
    2 -> Level Selector
  */

  static bool runOnce = true;
  if (runOnce) {
    headerSprite.setPosition(
        {(window->getSize().x - headerTexture.getSize().x) / 2.0f, 20});

    playButton.setPosition({920, 420});
    creditsButton.setPosition({860, 470});
    exitButton.setPosition({920, 520});
    sf::Mouse::setPosition({0, 0}, *window);

    runOnce = false;
  }

  switch (homeState) {
  case 0:
    // onClick(playButton, mousePosition);
    onClick(exitButton, []() { window->close(); });
    onClick(creditsButton, []() {
      homeState = 1;
      isOverlayChanged = true;
    });
    break;
  case 1:
    updateOverlay(7, names, "Credits", nullptr, []() { homeState = 0; }, "OK");
    break;
  }

  window->draw(backgroundSprite);
  window->draw(headerSprite);
  window->draw(playButton);
  window->draw(creditsButton);
  window->draw(exitButton);

  if (homeState == 1)
    drawOverlay();
}
