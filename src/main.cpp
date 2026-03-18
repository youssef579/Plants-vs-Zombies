#include <Game.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <globals.hpp>

sf::RenderWindow window(sf::VideoMode({1150, 606}), "Plants vs Zombies",
                        sf::Style::Close | sf::Style::Titlebar);

int main() {
  window.setFramerateLimit(60);

  sf::Image cursorImage("assets/cursor.png");
  sf::Cursor cursor(cursorImage.getPixelsPtr(), cursorImage.getSize(),
                    sf::Vector2u(0, 0));
  window.setMouseCursor(cursor);

  sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
  window.setPosition(sf::Vector2i((desktop.size.x - window.getSize().x) / 2,
                                  (desktop.size.y - window.getSize().y) / 2));

  sf::Image icon("assets/icon.png");
  window.setIcon(icon.getSize(), icon.getPixelsPtr());

  const auto onClose = [](const sf::Event::Closed &) { window.close(); };

  while (window.isOpen()) {
    window.handleEvents(onClose);

    window.clear();
    updateGame();
    window.display();
  }
}
