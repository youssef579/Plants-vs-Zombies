#include <Home.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <globals.hpp>

void updateHome() {
  sf::Texture backgroundTexture("assets/home.png");
  sf::Sprite backgroundSprite(backgroundTexture);

  sf::Texture headerTexture("assets/home_header.png");
  sf::Sprite headerSprite(headerTexture);
  headerSprite.setPosition(
      {(window.getSize().x - headerTexture.getSize().x) / 2.0f, 20});

  window.draw(backgroundSprite);
  window.draw(headerSprite);
}
