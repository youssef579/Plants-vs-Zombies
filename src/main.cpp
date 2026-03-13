#include <SFML/Graphics.hpp>

int main() {
  sf::RenderWindow window(sf::VideoMode({900, 600}), "Plants vs Zombies",
                          sf::Style::Close | sf::Style::Titlebar);
  window.setFramerateLimit(60);

  sf::CircleShape shape(100.f);
  shape.setFillColor(sf::Color::Green);

  const auto onClose = [&window](const sf::Event::Closed &) { window.close(); };

  while (window.isOpen()) {
    window.handleEvents(onClose);

    window.clear();
    window.draw(shape);
    window.display();
  }
}
