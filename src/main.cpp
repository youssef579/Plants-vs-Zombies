#include <Game.hpp>
#include <SFML/Graphics.hpp>
#include <globals.hpp>

sf::RenderWindow window(sf::VideoMode({1150, 606}), "Plants vs Zombies",
                        sf::Style::Close | sf::Style::Titlebar);

int main() {
  window.setFramerateLimit(60);

  sf::Image cursorImage("assets/cursor.png");
  sf::Cursor cursor(cursorImage.getPixelsPtr(), cursorImage.getSize(),
                    sf::Vector2u(0, 0));
  window.setMouseCursor(cursor);

  const auto onClose = [](const sf::Event::Closed &) { window.close(); };

  while (window.isOpen()) {
    window.handleEvents(onClose);

    window.clear();
    updateGame();
    window.display();
  }
}
